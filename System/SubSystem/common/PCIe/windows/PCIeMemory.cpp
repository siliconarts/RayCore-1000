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
// DATE    : 4/9/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "PCIeMemory.h"
#include "PCIeSystem.h"
#include <assert.h>

NativeMemory* CreateNativeMemory(DWORD dwByteSize){
	PCIeMemory* pMem	= new PCIeMemory();
	if(!pMem->Alloc(dwByteSize)){
		pMem->Release();
		pMem	= NULL;
	}
	return pMem;
}

PCIeMemory::PCIeMemory(void){
	m_hDriver	= PCIeSystem::GetCurrentDriverHandle();
	memset(&m_DMA, 0, sizeof(DMA_MEMORY_USER));
	memset(&m_trans, 0, sizeof(RC_TRANSACTION));
}

PCIeMemory::~PCIeMemory(void){
	if(m_DMA.hDMA.pointer){
		ULONG			bytesReturned;
		DeviceIoControl(
			m_hDriver,					// device handle
			IOCTL_RAYCORE_DMA_FREE,		// I/O Control code
			&m_DMA,						// in-buffer
			sizeof(DMA_MEMORY_USER),	// in-buffer size
			&m_DMA,						// out-buffer
			sizeof(DMA_MEMORY_USER),	// out-buffer size
			&bytesReturned,				// bytes returned
			NULL);						// overlaped
		m_DMA.hDMA.pointer	= NULL;
	}
}

BOOL PCIeMemory::Alloc(DWORD dwByteSize){
	m_DMA.byte_size	= dwByteSize;
	{
		ULONG			bytesReturned;
		if(!DeviceIoControl(
			m_hDriver,					// device handle
			IOCTL_RAYCORE_DMA_ALLOC,	// I/O Control code
			&m_DMA,						// in-buffer
			sizeof(DMA_MEMORY_USER),	// in-buffer size
			&m_DMA,						// out-buffer
			sizeof(DMA_MEMORY_USER),	// out-buffer size
			&bytesReturned,				// bytes returned
			NULL)						// overlaped
			){
			printf ( "Error in dma allocation.");
			return FALSE;
		}
	}

	m_pVirtual	= m_DMA.vir_addr.pointer;
	
	return TRUE;
}

void PCIeMemory::Reads(DWORD dwPhysical, DWORD dwOffset, DWORD dwByteSize){
	NativeSystem*	pSystem		= (PCIeSystem*)PCIeSystem::GetSystem();

	assert((dwPhysical&(32 - 1)) == 0);			// must 32 byte(128bit) align
	assert((dwOffset&(8 - 1)) == 0);			// must 8 byte(64bit) align

	m_trans.device_id	= (unsigned short)pSystem->GetCurrentCardID();
	m_trans.offset		= dwOffset;
	m_trans.byte_size	= dwByteSize;
	m_trans.phy_address	= dwPhysical;

	{
		ULONG			bytesReturned;
		DeviceIoControl(
			m_hDriver,						// device handle
			IOCTL_RAYCORE_TRANSACTION_DMA,	// I/O Control code
			&m_trans,						// in-buffer
			sizeof(RC_TRANSACTION),			// in-buffer size
			&m_DMA,							// out-buffer
			sizeof(DMA_MEMORY_USER),		// out-buffer size
			&bytesReturned,					// bytes returned
			NULL);							// overlaped
	}
}

void PCIeMemory::Wait(void){
	NativeSystem*	pSystem		= (PCIeSystem*)PCIeSystem::GetSystem();
	
	m_trans.device_id	= (unsigned short)pSystem->GetCurrentCardID();
	m_trans.byte_size	= 0;
	
	{
		ULONG			bytesReturned;
		DeviceIoControl(
			m_hDriver,						// device handle
			IOCTL_RAYCORE_TRANSACTION_DMA,	// I/O Control code
			&m_trans,						// in-buffer
			sizeof(RC_TRANSACTION),			// in-buffer size
			&m_DMA,							// out-buffer
			sizeof(DMA_MEMORY_USER),		// out-buffer size
			&bytesReturned,					// bytes returned
			NULL);							// overlaped
	}
}
