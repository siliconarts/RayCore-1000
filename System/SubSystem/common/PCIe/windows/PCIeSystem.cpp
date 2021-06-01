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
// DATE    : 11/14/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "PCIeSystem.h"
#include "SystemMemory.h"
#include <stdio.h>
#include <assert.h>
#ifdef USE_LICENSE_MANAGER
#include "LicenseManager.h"
#endif

PCIeSystem*	PCIeSystem::m_pCurrent	= NULL;

PCIeSystem::PCIeSystem(void){
	m_dwCurrentCardID	= 0;
	ZeroMemory(&m_DriverInfo, sizeof(RC_DRIVER_INFO));
	ZeroMemory(&m_Transaction, sizeof(RC_TRANSACTION));

	// 드라이버 열기
	m_hDriver	= CreateFile(	DEV_PATH,
								GENERIC_READ | GENERIC_WRITE,
								0,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	m_pCurrent	= this;
}

PCIeSystem::~PCIeSystem(void){
	if(m_hDriver != INVALID_HANDLE_VALUE){
		CloseHandle(m_hDriver);
		m_hDriver	= INVALID_HANDLE_VALUE;
	}
	m_pCurrent	= NULL;
}

BOOL PCIeSystem::OnSystemInitialize(void){
#ifdef USE_LICENSE_MANAGER
	// license check
	if(!g_LicenseManager.CheckLicense() || GetKeyState(VK_RSHIFT)<0){
		g_LicenseManager.RunModal();
	}
#endif
	if(m_hDriver == INVALID_HANDLE_VALUE) return FALSE;
	
	// 정보 얻기
	{
		ULONG			bytesReturned;
		if(!DeviceIoControl(
			m_hDriver,					// device handle
			IOCTL_RAYCORE_INFOMATION,	// I/O Control cde
			&m_DriverInfo,				// in-buffer
			sizeof(RC_DRIVER_INFO),		// in-buffer size
			&m_DriverInfo,				// out-buffer
			sizeof(RC_DRIVER_INFO),		// out-buffer size
			&bytesReturned,				// bytes returned
			NULL)){						// overlaped
			return FALSE;
		}
		m_dwCardCount	= m_DriverInfo.card_count;
	}
#ifdef USE_LICENSE_MANAGER
	{	// license limitation
		if(g_LicenseManager.CheckLicense())
			printf("Current available card usability license : %d\n",  g_LicenseManager.CheckLicense());
		else
			printf("No license is found.\n");

		if(m_dwCardCount > 1 && m_dwCardCount > (DWORD)g_LicenseManager.CheckLicense()){
			// 보드가 1개 이상일 경우 라이센스 개수에 따라 제한함.
			m_dwCardCount	= g_LicenseManager.CheckLicense();
		}
	}
#endif
	{	// PCIe payload 초기화
		printf(	"───────────────────────────────────────\n" \
				" ◈ Device information.\n" \
				"\t1. Card connection info.\n");

		for(DWORD i=0;i<m_dwCardCount;i++){
			RC_DEVICE_INFO*	pDevice	= &m_DriverInfo.device[i];
			m_DriverInfo.device[i].pcie.gen;

			printf("\t\t(%d/%d) Sub-System : VID(0x%04X), DID(0x%04X), PCIe %d.0 - %d Lanes\n", i+1, m_dwCardCount, pDevice->sub_system&0xFFFF, (pDevice->sub_system>>16)&0xFFFF, pDevice->pcie.gen, pDevice->pcie.lanes);

			switch(pDevice->sub_system){
			case 0x10121172:	printf("\t\t\tArriaV board system : 84MHz, 2 Cores.\n");		break;
			case 0x10321172:	printf("\t\t\tStratix board system : 100MHz, 4 Cores.\n");		break;
			default:			printf("\t\t\tUnknown system!\n");								break;
			}
		}
		printf("───────────────────────────────────────\n\n");
		SetCurrentCard(0);
#if defined(WIN32) || defined(WIN64)
		_flushall();
#endif
	}

	return TRUE;
}

DWORD PCIeSystem::GetCardCount(void){
	return m_dwCardCount;
}

BOOL PCIeSystem::SetCurrentCard(DWORD dwCardID){
	if(m_dwCurrentCardID>=m_dwCardCount) return FALSE;
	m_Transaction.device_id	= (unsigned short)dwCardID;
	m_dwCurrentCardID		= dwCardID;
	return TRUE;
}

DWORD PCIeSystem::GetCurrentCardID(void){
	return m_dwCurrentCardID;
}

DWORD PCIeSystem::GetCurrentSubsystemID(void){
	return m_DriverInfo.device[m_dwCurrentCardID].sub_system;
}

void PCIeSystem::RegReads(DWORD Offset32, DWORD* pData, int iCount){
	m_Transaction.is_write		= 0;
	m_Transaction.is_memory		= 0;
	m_Transaction.byte_size		= iCount << 2;
	m_Transaction.offset		= Offset32 << 2;

	TryTransaction(pData, m_Transaction.byte_size);
}

void PCIeSystem::RegWrites(DWORD Offset32, const DWORD* pData, int iCount){
	m_Transaction.is_write		= 1;
	m_Transaction.is_memory		= 0;
	m_Transaction.byte_size		= iCount << 2;
	m_Transaction.offset		= Offset32 << 2;

	TryTransaction((DWORD*)pData, m_Transaction.byte_size);
}

void PCIeSystem::MemWrites(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize 는 16*DWORD 의 배수이어야 한다.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	m_Transaction.is_write		= 1;
	m_Transaction.is_memory		= 1;
	m_Transaction.byte_size		= dwByteSize;
	m_Transaction.phy_address	= dwPhysical;

	TryTransaction((DWORD*)pData, m_Transaction.byte_size);
}

void PCIeSystem::MemReads(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize){
	dwByteSize			+= (dwPhysical&0x3F) + 0x3F;	// dwByteSize 는 16*DWORD 의 배수이어야 한다.
	pData				= (DWORD*)(((BYTE*)pData) - (dwPhysical&0x3F));

	m_Transaction.is_write		= 0;
	m_Transaction.is_memory		= 1;
	m_Transaction.byte_size		= dwByteSize;
	m_Transaction.phy_address	= dwPhysical;

	TryTransaction((DWORD*)pData, m_Transaction.byte_size);
}

BOOL PCIeSystem::TryTransaction(DWORD* pData, DWORD dwByteSize){
	ULONG			bytesReturned;
	if(!DeviceIoControl(
		m_hDriver,					// device handle
		IOCTL_RAYCORE_TRANSACTION,	// I/O Control cde
		&m_Transaction,				// in-buffer
		sizeof(RC_TRANSACTION),		// in-buffer size
		pData,						// out-buffer
		dwByteSize,					// out-buffer size
		&bytesReturned,				// bytes returned
		NULL)){						// overlaped
		return FALSE;
	}
	return TRUE;
}

void PCIeSystem::DMAFlush(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize){
	DWORD dwPhysical		= pMem->Physical() + dwOffsetPhy;
	NativeMemory* pNative	= pMem->GetNativeMemory();

#ifdef USE_LICENSE_MANAGER
	g_LicenseManager.CheckTrialLoop();
#endif
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
