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

#ifndef __PCIE_MEMORY_H__
#define __PCIE_MEMORY_H__

#include "NativeInterface.h"
#include "driver_raycore.h"

class PCIeMemory :
	public NativeMemory
{
	friend NativeMemory* CreateNativeMemory(DWORD dwByteSize);
protected:
	virtual ~PCIeMemory(void);

public:
	PCIeMemory(void);

	STDMETHOD_(void, Reads)(DWORD dwPhysical, DWORD dwOffset, DWORD dwByteSize);
	STDMETHOD_(void, Wait)(void);

protected:
	STDMETHOD_(BOOL, Alloc)(DWORD dwByteSize);

private:
	HANDLE				m_hDriver;
	DMA_MEMORY_USER		m_DMA;
	RC_TRANSACTION		m_trans;
};

#endif//__PCIE_MEMORY_H__
