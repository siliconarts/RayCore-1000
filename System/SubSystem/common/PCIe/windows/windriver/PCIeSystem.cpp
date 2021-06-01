//=======================================================================
//
// This confidential and proprietary software may be used only as
// authorized by a licensing agreement from Siliconarts Inc.
//
//   (C) Copyright 2010 ~ 2013  Siliconarts Inc. ALL RIGHTS RESERVED
//
// The entire notice above must be reproduced on all authorized
// copies and copies may only be made to the extent permitted
// by a licensing agreement from Siliconarts.
//
// Project : PCIe common
// DATE    : 3/28/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "PCIeSystem.h"
#include "SystemMemory.h"
#include <stdio.h>
#include <assert.h>

// typedef struct{
// 	IDMAMemory*		pDMA;
// 	BOOL			bMustSync;
// }card_context;

// static void RayCore_INT_HANDLER(WDC_DEVICE_HANDLE hDev, PCIe_INT_RESULT *pIntResult)
// {
// 	printf("Got interrupt number %ld\n", pIntResult->dwCounter);
// }

PCIeSystem::PCIeSystem(void){
}

PCIeSystem::~PCIeSystem(void){
}

BOOL PCIeSystem::OnSystemInitialize(void){
	if(!m_PCIe.Initialize(0x1172, 0x1000)) return FALSE;
	m_dwCardCount	= m_PCIe.GetCardCount();
	{	// PCIe payload 초기화
		REG_PCIE_CAPABILITY		cap;
		ZeroMemory(&cap, sizeof(REG_PCIE_CAPABILITY));
		printf(	"───────────────────────────────────────\n" \
				" ◈ Device information.\n" \
				"\t1. Card connection info.\n");

		for(DWORD i=0;i<m_PCIe.GetCardCount();i++){
			DWORD	LinkStatus, SubSystem;
			int		iGen, iLane;
			m_PCIe.SetCurrentCard(i);
			m_PCIe.ReadConfigRegister(0x90, &LinkStatus, sizeof(DWORD));
			m_PCIe.ReadConfigRegister(0x2C, &SubSystem, sizeof(DWORD));

			iGen	= (LinkStatus>>16)&0xF;
			iLane	= (LinkStatus>>20)&0xF;

			printf("\t\t(%d/%d) Sub-System : VID(0x%04X), DID(0x%04X), PCIe %d.0 - %d Lanes\n", i+1, m_PCIe.GetCardCount(), SubSystem&0xFFFF, (SubSystem>>16)&0xFFFF, iGen, iLane);

			m_dwSubsystemID[i]	= SubSystem;

			switch(SubSystem){
			case 0x10121172:	printf("\t\t\tArriaV board system : 84MHz, 2 Cores.\n");		break;
			case 0x10321172:	printf("\t\t\tStratix board system : 100MHz, 3 Cores.\n");		break;
			default:			printf("\t\t\tUnknown system!\n");								break;
			}

			{	// setting payload size
//				DWORD device_capabilities;
				m_PCIe.ReadConfigRegister(0x84, &cap.device_capabilities, sizeof(DWORD)*2);
//				printf("max_payload_size_support = %d\n", 128 * (1<<cap.device_capabilities.max_payload_size_support));

				cap.device_control.max_payload_size			= cap.device_capabilities.max_payload_size_support;
				cap.device_control.max_read_request_size	= 5;	// 4K size : read request 의 경우 break 이 가능하기 때문에 최대한 크게 잡는다.
				m_PCIe.WriteConfigRegister(0x88, &cap.device_control, sizeof(WORD));

//				m_PCIe.ReadConfigRegister(0x84, &cap.device_capabilities, sizeof(DWORD)*2);
//				printf("current max payload = %d\n", 128 * (1<<cap.device_control.max_payload_size));
//				printf("current max_read_request_size = %d\n", 128 * (1<<cap.device_control.max_read_request_size));

// 				REG_PCIE_COMMON common;
// 				m_PCIe.ReadConfigRegister(0x04, &common.command, sizeof(WORD));
// 				printf("bus_master_enable = %d\n", common.command.bus_master_enable);
			}

		}
		printf("───────────────────────────────────────\n\n");
#ifdef WIN32
		_flushall();
#endif
	}


	// initialize card context
// 	{
// 		if(m_pContext) delete [] m_pContext;
// 		m_pContext	= new card_context[GetCardCount()];
// 		ZeroMemory(m_pContext, sizeof(card_context)*GetCardCount());
// 	}

	// DMA Test
#if 0
	{
		IDMAMemory*	pMem		= CreateDMAMemory(1024*1024*8);
		if(pMem){

			DWORD data[2048];

			//DDR test data Load
			for(int i=0;i<2048;i++) data[i]=i+1 + 0x100;
			MemoryWrite(0, (unsigned int*)data, 512);
			MemoryWrite(32, &data[512], 512);
			MemoryWrite(64, &data[1024], 512);
			MemoryWrite(96, &data[1536], 512);
			MemoryWrite((1024*1024 - 16*4)/64, (unsigned int*)data, 512);

			// DMA transfer
			DMAPostRead(0, pMem, 0, 1024*1024);

			// Sync.
			DMACheckSync(pMem);

			// Read
			for(int i=0;i<16;i++)
				printf("D[%d] = 0x%X\n", i, ((DWORD*)pMem->Virtual())[i]);

			for(int i=0;i<16;i++)
				printf("D[%d] = 0x%X\n", i, ((DWORD*)pMem->Virtual())[((1024*1024)/4) - 16 + i]);

			// Release
			pMem->Release();
		}else{
			printf("DMA failed\n");
		}
	}
#endif
	return TRUE;
}

DWORD PCIeSystem::GetCardCount(void){
	return m_dwCardCount;
}

BOOL PCIeSystem::SetCurrentCard(DWORD dwCardID){
	return m_PCIe.SetCurrentCard(dwCardID);
}

DWORD PCIeSystem::GetCurrentCardID(void){
	return m_PCIe.GetCurrentCardID();
}

DWORD PCIeSystem::GetCurrentSubsystemID(void){
	return m_dwSubsystemID[m_PCIe.GetCurrentCardID()];
}

void PCIeSystem::RegReads(DWORD Offset32, DWORD* pData, int iCount){
	//m_PCIe.BlockRead(0, dwAddr, pData, dwSize*sizeof(DWORD));
	DWORD	cmd		= (1<<31)|(0<<30)|(iCount<<23)|Offset32;

	m_PCIe.Write32(0, 0x2000, cmd);
	//m_PCIe.BlockReadFIFO64(0, 0, pData, iCount*sizeof(DWORD));
	m_PCIe.BlockRead(0, 0, pData, iCount*sizeof(DWORD));
}

void PCIeSystem::RegWrites(DWORD Offset32, const DWORD* pData, int iCount){
	//m_PCIe.BlockWrite(0, dwAddr, pData, dwSize*sizeof(DWORD));
	DWORD	cmd		= (1<<31)|(1<<30)|(iCount<<23)|Offset32;

	m_PCIe.Write32(0, 0x2000, cmd);
	m_PCIe.BlockWriteFIFO64(0, 0, (PVOID)pData, iCount*sizeof(DWORD));
}

void PCIeSystem::MemWrites(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize 는 16*DWORD 의 배수이어야 한다.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	{
		DWORD	cmd		= (1<<30)|(16<<23)|(dwPhysical >> (4+2));
		DWORD	dwCount	= dwByteSize >> (4+2);

		for(DWORD i=0;i<dwCount;i++){
			m_PCIe.Write32(0, 0x2000, cmd);cmd++;
			// Memory write
			m_PCIe.BlockWriteFIFO64(0, 0, pData, 16*sizeof(DWORD));
			pData	+= 16;
		}
	}
}

void PCIeSystem::MemReads(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize 는 16*DWORD 의 배수이어야 한다.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	{
		DWORD	cmd		= (0<<30)|(16<<23)|(dwPhysical >> (4+2));
		DWORD	dwCount	= dwByteSize >> (4+2);

		for(DWORD i=0;i<dwCount;i++){
			m_PCIe.Write32(0, 0x2000, cmd);cmd++;
			// Memory read
			m_PCIe.BlockRead(0, 0, pData, 16*sizeof(DWORD));
			pData	+= 16;
		}
	}
}

void PCIeSystem::DMAFlush(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize){
	DWORD dwPhysical		= pMem->Physical() + dwOffsetPhy;
	NativeMemory* pNative	= pMem->GetNativeMemory();

	if(pNative && !bCopyToPhysical){	// Support DMA read operation only...
		pNative->Reads(dwPhysical, dwOffsetPhy + dwOffsetVir, dwByteSize);
	}else{
		BYTE* pVirtual		= ((BYTE*)pMem->Virtual()) + dwOffsetPhy + dwOffsetVir;
		if(bCopyToPhysical)
			MemWrites(dwPhysical, (DWORD*)pVirtual, dwByteSize);
		else
			MemReads(dwPhysical, (DWORD*)pVirtual, dwByteSize);
	}
}
