// Copyright(c) 2010 ~ 2013 Siliconarts, Inc. All Rights Reserved
// RayCore DDK v 1.0
// Date : 

#include "STDInterface.h"
#include "stdlib.h"
#include "PCIeDriver.h"
#include "windrvr_int_thread.h"
//#include "status_strings.h"
#include <stdio.h>

#define INTCSR 0x00
#define INTCSR_ADDR_SPACE 0
#define ALL_INT_MASK ((BYTE)0xFF)
#define PCIe_DEFAULT_LICENSE_STRING	"6C3CC2CFE89E7AD042444C655646F21A0BF3FDEB.EMBRACE"
#define PCIe_DEFAULT_DRIVER_NAME	"windrvr6"

typedef struct {
	WD_TRANSFER*		pIntTransCmds;
    PCIe_INT_HANDLER	funcDiagIntHandler;
    PCIe_EVENT_HANDLER	funcDiagEventHandler;
} DEV_CTX, *PDEV_CTX;

static BOOL				g_bWindriverOpen	= FALSE;
WDC_DEVICE_HANDLE		g_hPCIeDev			= NULL;
WD_PCI_CARD_INFO		g_hPCIeDeviceInfo	= {0};


static BOOL IsItemExists(PWDC_DEVICE pDev, ITEM_TYPE item)
{
    DWORD i, dwNumItems = pDev->cardReg.Card.dwItems;

    for (i=0; i<dwNumItems; i++)
    {
        if (pDev->cardReg.Card.Item[i].item == item)
            return TRUE;
    }

    return FALSE;
}

static void DLLCALLCONV DEMO_IntHandler(PVOID pData)
{
	PWDC_DEVICE pDev = (PWDC_DEVICE)pData;
	PDEV_CTX pDevCtx = (PDEV_CTX)WDC_GetDevContext(pDev);
	PCIe_INT_RESULT intResult;

	BZERO(intResult);
	intResult.dwCounter = pDev->Int.dwCounter;
	intResult.dwLost = pDev->Int.dwLost;
	intResult.waitResult = (WD_INTERRUPT_WAIT_RESULT)pDev->Int.fStopped;
	intResult.dwEnabledIntType = WDC_GET_ENABLED_INT_TYPE(pDev);
	intResult.dwLastMessage = WDC_GET_ENABLED_INT_LAST_MSG(pDev);

	/* Execute the diagnostics application's interrupt handler routine */
	pDevCtx->funcDiagIntHandler((WDC_DEVICE_HANDLE)pDev, &intResult);
}


BOOL PCIeInterruptEnable(PCIe_INT_HANDLER funcIntHandler){
	DWORD			dwStatus;
	PWDC_DEVICE		pDev				= (PWDC_DEVICE)g_hPCIeDev;
	PDEV_CTX		pDevCtx;
	WDC_ADDR_DESC*	pAddrDesc;
	WD_TRANSFER*	pTrans				= NULL;
	DWORD			dwNumTransCmds		= 0;
	DWORD			dwOptions			= 0;

	if(!g_hPCIeDev || !funcIntHandler) return FALSE;
	if (!IsItemExists(pDev, ITEM_INTERRUPT)){
		printf("사용 가능한 인터럽트가 없습니다.\n");
		return FALSE;
	}
	
	if(WDC_IntIsEnabled(g_hPCIeDev)){
		printf("Interrupts are already enabled...\n");
		return FALSE;
	}

	pDevCtx = (PDEV_CTX)WDC_GetDevContext(pDev);

	/* In case of kernel plugin, interrupts acknowledged in kernel mode */
	if (!WDC_IS_KP(pDev)){
		/* TODO: Change this value, if needed */
		dwNumTransCmds = 2;

        /* Allocate memory for the interrupt transfer commands */
		pTrans = (WD_TRANSFER*)calloc(dwNumTransCmds, sizeof(WD_TRANSFER));
		if (!pTrans)
		{
			printf("Failed allocating memory for interrupt transfer commands\n");
			return FALSE;
		}
		/* Prepare the interrupt transfer commands.
         *
         * The transfer commands will be executed by WinDriver's ISR
         * which runs in kernel mode at interrupt level.
         */

        /* TODO: Change the offset of INTCSR and the PCI address space, if
         * needed */
        /* #1: Read status from the INTCSR register */
        pAddrDesc = WDC_GET_ADDR_DESC(pDev, INTCSR_ADDR_SPACE);

		pTrans[0].dwPort = pAddrDesc->kptAddr + INTCSR;
		/* read 32bit register */
		pTrans[0].cmdTrans = WDC_ADDR_IS_MEM(pAddrDesc) ? RM_DWORD : RP_DWORD; 

		/* #2: Write ALL_INT_MASK to the INTCSR register to acknowledge the
         * interrupt */
        pTrans[1].dwPort = pTrans[0].dwPort; /* In this example both commands
                                                access the same address
                                                (register) */
		/* write 32bit register */
		pTrans[1].cmdTrans = WDC_ADDR_IS_MEM(pAddrDesc) ? WM_DWORD : WP_DWORD; 
		pTrans[0].Data.Dword = ALL_INT_MASK;
		
		/* copy the results of "read" transfer commands back to user mode */
		dwOptions = INTERRUPT_CMD_COPY;
	}

	/* Store the diag interrupt handler routine, which will be executed by
       PCI_IntHandler() when an interrupt is received */
    pDevCtx->funcDiagIntHandler = funcIntHandler;

	/* Enable the interrupts */
	dwStatus = WDC_IntEnable(g_hPCIeDev, pTrans, dwNumTransCmds, dwOptions,
		DEMO_IntHandler, (PVOID)pDev, WDC_IS_KP(pDev));

	pDevCtx->pIntTransCmds		= pTrans;

	//if(WDC_IntEnable(g_hPCIeDev, NULL, 0, 0, DEMO_IntHandler, (PVOID)pDev, WDC_IS_KP(g_hPCIeDev)) != WD_STATUS_SUCCESS) return FALSE;
	if(dwStatus != WD_STATUS_SUCCESS){
		//printf("Failed enabling interrupts. Error 0x%lx - %s\n", dwStatus, Stat2Str(dwStatus));
		free(pTrans);
		return FALSE;
	}

	return TRUE;
}

void PCIeInterruptDisable(void){
	if(!g_hPCIeDev || !WDC_IntIsEnabled(g_hPCIeDev)) return;
	PDEV_CTX pDevCtx = (PDEV_CTX)WDC_GetDevContext(g_hPCIeDev);
	if(WDC_IntDisable(g_hPCIeDev) != WD_STATUS_SUCCESS){
		printf("Failed disabling interrupts.\n");
		return;
	}
	if(pDevCtx->pIntTransCmds){
		free(pDevCtx->pIntTransCmds);
		pDevCtx->pIntTransCmds	= NULL;
	}
}


/* -----------------------------------------------
    Plug-and-play and power management events
   ----------------------------------------------- */
static void DEMO_EventHandler(WD_EVENT *pEvent, PVOID pData)
{
    PWDC_DEVICE pDev = (PWDC_DEVICE)pData;
    PDEV_CTX pDevCtx = (PDEV_CTX)WDC_GetDevContext(pDev);

    //TraceLog("DEMO_EventHandler entered, pData: 0x%p, dwAction 0x%lx\n", pData, pEvent->dwAction);

	// Execute the diagnostics application's event handler function
    pDevCtx->funcDiagEventHandler((WDC_DEVICE_HANDLE)pDev, pEvent->dwAction);
}

BOOL PCIeEventRegister(PCIe_EVENT_HANDLER funcEventHandler){
	PWDC_DEVICE		pDev		= (PWDC_DEVICE)g_hPCIeDev;
	PDEV_CTX		pDevCtx;
	DWORD dwActions = WD_ACTIONS_ALL;

	PCIeEventUnregister();

    /* TODO: Modify the above to set up the plug-and-play/power management
             events for which you wish to receive notifications.
             dwActions can be set to any combination of the WD_EVENT_ACTION
             flags defined in windrvr.h */

	if(!g_hPCIeDev || !funcEventHandler) return FALSE;
	

	pDevCtx = (PDEV_CTX)WDC_GetDevContext(pDev);
	pDevCtx->funcDiagEventHandler = funcEventHandler;

	if(WDC_EventRegister(g_hPCIeDev, dwActions, DEMO_EventHandler, g_hPCIeDev, FALSE) != WD_STATUS_SUCCESS) return FALSE;

	return TRUE;
}

void PCIeEventUnregister(void){
	if(!g_hPCIeDev) return;
	if (WDC_EventIsRegistered(g_hPCIeDev)) WDC_EventUnregister(g_hPCIeDev);
}



PCIeDriver*		PCIeDriver::m_pPCIeDriver	= NULL;

PCIeDriver::PCIeDriver(void){
	m_pCard				= NULL;
	m_pCurrentCard		= NULL;
	m_hWindriver		= NULL;
	m_dwCardCount		= 0;
	m_dwCurrenCardID	= 0;
	m_bInitialized		= FALSE;
	ZeroMemory(&m_PCIeConfig, sizeof(WD_PCI_CONFIG_DUMP));
	m_pPCIeDriver		= this;
}



PCIeDriver::~PCIeDriver(void){
	m_pPCIeDriver	= NULL;
	Release();
}


BOOL PCIeDriver::Initialize(DWORD dwVendorId, DWORD dwDeviceId){
	Release();
	//---------------------------------------------------------------------
	// Set default windriver version name
	if(!WD_DriverName(PCIe_DEFAULT_DRIVER_NAME)) return FALSE;
	// Set WDC library's debug options (default: level TRACE, output to Debug Monitor)
	if (WDC_SetDebugOptions(WDC_DBG_DEFAULT, NULL) != WD_STATUS_SUCCESS) return FALSE;
	// Open a handle to the driver and initialize the WDC library
	if (WDC_DriverOpen(WDC_DRV_OPEN_DEFAULT, PCIe_DEFAULT_LICENSE_STRING) != WD_STATUS_SUCCESS) return FALSE;
	m_bInitialized	= TRUE;

	m_hWindriver	= WD_Open();

	//---------------------------------------------------------------------
	// Initializing our driver
	{
		// PCIe 장치를 검색하여 찾는다.
		WDC_PCI_SCAN_RESULT scanResult;
		BZERO(scanResult);
		WDC_PciScanDevices(dwVendorId, dwDeviceId, &scanResult);
		m_dwCardCount	= scanResult.dwNumDevices;
		if(!m_dwCardCount) return FALSE;	// No device found.


		// 자원 할당.
		m_pCurrentCard	=
		m_pCard			= new PCIeCard[m_dwCardCount];
		ZeroMemory(m_pCard, sizeof(PCIeCard)*m_dwCardCount);

		for(DWORD i=0;i<m_dwCardCount;i++){
			SetCurrentCard(i);
			m_pCurrentCard->Info.pciSlot	= scanResult.deviceSlot[i];
			WDC_PciGetDeviceInfo(&m_pCurrentCard->Info);
			{	// 드라이버 열기
				PDEV_CTX pDevCtx	= NULL;
				pDevCtx				= (PDEV_CTX)(new DEV_CTX);//malloc(sizeof (DEV_CTX));
				BZERO(*pDevCtx);
				WDC_PciDeviceOpen(&m_pCurrentCard->hDev, &m_pCurrentCard->Info, pDevCtx, NULL, NULL, NULL);
				if(!m_pCurrentCard->hDev){
					m_dwCardCount = i;
					Release();
					printf("*E : Can't open device. : Maybe it's device was opened by another program...\n");
					return FALSE;
				}
			}
		}
	}
	SetCurrentCard(0);
	
	return TRUE;
}

void PCIeDriver::Release(void){
	if(m_pCard){
		for(DWORD i=0;i<m_dwCardCount;i++){
			WDC_DEVICE_HANDLE	hDev	= m_pCard[i].hDev;
			PWDC_DEVICE			pDev	= (PWDC_DEVICE)hDev;
			PDEV_CTX			pDevCtx;
			if(WDC_IntIsEnabled(hDev)) WDC_IntDisable(hDev);	// Interrupt Disable
			pDevCtx = (PDEV_CTX)WDC_GetDevContext(pDev);
			WDC_PciDeviceClose(hDev);		// g_hPCIeDev 를 해제 시킨다.
			if(pDevCtx) delete pDevCtx;
		}
		delete [] m_pCard;
		m_pCard			= NULL;
		m_pCurrentCard	= NULL;
		m_dwCardCount	= 0;
	}
	if(m_hWindriver){
		WD_Close(m_hWindriver);
		m_hWindriver	= NULL;
	}
	if(m_bInitialized){
		WDC_DriverClose();
		m_bInitialized	= FALSE;
	}
}

BOOL PCIeDriver::IsInitialized(void){
	return m_dwCardCount != 0;
}

BOOL PCIeDriver::SetCurrentCard(DWORD dwCard){
	if(dwCard >= m_dwCardCount) return FALSE;
	m_dwCurrenCardID		= dwCard;
	m_pCurrentCard			= &m_pCard[dwCard];
	m_PCIeConfig.pciSlot	= m_pCurrentCard->Info.pciSlot;
	return TRUE;
}

BOOL PCIeDriver::WriteConfigRegister(DWORD dwOffset, PVOID pData, DWORD dwByteSize){
	if(!pData || !m_pCurrentCard) return FALSE;

	m_PCIeConfig.pBuffer		= pData;
	m_PCIeConfig.dwOffset		= dwOffset;
	m_PCIeConfig.dwBytes		= dwByteSize;
	m_PCIeConfig.fIsRead		= FALSE;

	WD_PciConfigDump(m_hWindriver, &m_PCIeConfig);

	return TRUE;
}

BOOL PCIeDriver::ReadConfigRegister(DWORD dwOffset, PVOID pData, DWORD dwByteSize){
	if(!pData || !m_pCurrentCard) return FALSE;

	m_PCIeConfig.pBuffer		= pData;
	m_PCIeConfig.dwOffset		= dwOffset;
	m_PCIeConfig.dwBytes		= dwByteSize;
	m_PCIeConfig.fIsRead		= TRUE;

	WD_PciConfigDump(m_hWindriver, &m_PCIeConfig);

	return TRUE;
}

BOOL PCIeDriver::InterruptEnable(PCIe_INT_HANDLER funcIntHandler){
	return TRUE;
}

void PCIeDriver::InterruptDisable(void){
}

void* PCIeDriver::GetUserPointer(int bar, int address){
	WDC_ADDR_DESC *pAddrDesc = WDC_GET_ADDR_DESC(m_pCurrentCard->hDev, bar);
	return (void*)(&(((BYTE*)(WDC_MEM_DIRECT_ADDR(pAddrDesc)))[address]));
}

UINT64 PCIeDriver::GetPhyAddress(int bar, int address){
	union{
		UINT64		addres;
		struct{
			DWORD	low;
			DWORD	hi;
		};
	}base;

	DWORD bar_config_base = 0x010 + bar*4;

	ReadConfigRegister(bar_config_base, &base.low, sizeof(DWORD));

	if((base.low&0xF) == 0xC){
		// 64 bit address
		ReadConfigRegister(bar_config_base + 4, &base.hi, sizeof(DWORD));
	}else{
		// 32 bit legacy address
		base.hi	= 0;
	}
	base.low &= ~0xF;

	//WDC_ADDR_DESC *pAddrDesc = WDC_GET_ADDR_DESC(m_pCurrentCard->hDev, bar);
	//return m_pCurrentCard->Info.Card.Item[pAddrDesc->dwItemIndex].I.Mem.dwPhysicalAddr + address;
	return base.addres + address;
}

DWORD PCIeDriver::Read32(DWORD dwBar, DWORD dwAddr){
	return *((DWORD*)GetUserPointer(dwBar, dwAddr));
}

void PCIeDriver::Write32(DWORD dwBar, DWORD dwAddr, DWORD dwData){
	//*((DWORD*)GetUserPointer(dwBar, dwAddr)) = dwData;
	WDC_WriteAddr32(m_pCurrentCard->hDev, dwBar, dwAddr, dwData);
}

void PCIeDriver::BlockRead(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_ReadAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_64, WDC_ADDR_RW_DEFAULT);
}

void PCIeDriver::BlockWrite(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_WriteAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_64, WDC_ADDR_RW_DEFAULT);
}

void PCIeDriver::BlockReadFIFO32(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_ReadAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_32, WDC_ADDR_RW_NO_AUTOINC);
}

void PCIeDriver::BlockWriteFIFO32(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_WriteAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_32, WDC_ADDR_RW_NO_AUTOINC);
}

void PCIeDriver::BlockReadFIFO64(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_ReadAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_64, WDC_ADDR_RW_NO_AUTOINC);
}

void PCIeDriver::BlockWriteFIFO64(DWORD dwBar, DWORD dwAddr, PVOID pData, DWORD dwByteSize){
	WDC_WriteAddrBlock(m_pCurrentCard->hDev, dwBar, dwAddr, dwByteSize, pData, WDC_MODE_64, WDC_ADDR_RW_NO_AUTOINC);
}
