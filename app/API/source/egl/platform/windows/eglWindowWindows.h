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
// Project : API
// DATE    : 4/19/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_WINDOW_WINDOWS_H__
#define __EGL_WINDOW_WINDOWS_H__
#include "../eglWindow.h"

#ifdef USE_TESTDRIVE
#include "TestDriver.h"
#include "driver/SystemConfig.h"
#include "VirtualDisplayConfig.h"
#endif

class eglWindowWindows :
	public eglWindow
{
public:
	eglWindowWindows(EGLNativeWindowType win);
	virtual ~eglWindowWindows(void);

	virtual void GetWindowSize(int &iWidth, int &iHeight);
	virtual void Present(RCDDK* pDDK, DrawBuffer* pBuffer);

	inline BOOL IsInitialized(void)	{return (m_lpDIBits != NULL);}

private:
	int					m_iWidth, m_iHeight;
	HWND				m_hWnd;
	HDC					m_hDC;
	HDC					m_memDC;
	HBITMAP				m_hbmScreen;
	LPVOID				m_lpDIBits;
	BITMAPV5HEADER		m_bmpHeader;

#ifdef USE_TESTDRIVE
	ITestDriverMemory*	m_pTDMemory_Display;
	DisplayConfig*		m_pDisplayConfig;
#endif
};

#endif//__EGL_WINDOW_WINDOWS_H__
