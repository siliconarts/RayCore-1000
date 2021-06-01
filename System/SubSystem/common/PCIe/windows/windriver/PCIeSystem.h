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

#ifndef __PCIE_SYSTEM_H__
#define __PCIE_SYSTEM_H__

#include "NativeInterface.h"
#include "PCIeDriver.h"

class PCIeSystem :
	public NativeSystem
{
public:
	PCIeSystem(void);
	virtual ~PCIeSystem(void);

	STDMETHOD_(DWORD, GetCardCount)(void);
	STDMETHOD_(BOOL, SetCurrentCard)(DWORD dwCardID);
	STDMETHOD_(DWORD, GetCurrentCardID)(void);
	STDMETHOD_(DWORD, GetCurrentSubsystemID)(void);
	STDMETHOD_(void, RegReads)(DWORD Offset32, DWORD* pData, int iCount);
	STDMETHOD_(void, RegWrites)(DWORD Offset32, const DWORD* pData, int iCount);
	STDMETHOD_(void, MemReads)(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize);
	STDMETHOD_(void, MemWrites)(DWORD dwPhysical, DWORD* pData, DWORD dwByteSize);
	STDMETHOD_(void, DMAFlush)(BOOL bCopyToPhysical, SystemMemory* pMem, DWORD dwOffsetPhy, DWORD dwOffsetVir, DWORD dwByteSize);

protected:
	STDMETHOD_(BOOL, OnSystemInitialize)(void);
	
private:
	DWORD				m_dwSubsystemID[16];
	PCIeDriver			m_PCIe;
};

#endif//__PCIE_SYSTEM_H__
