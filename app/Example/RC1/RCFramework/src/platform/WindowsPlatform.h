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
// Project : Example
// DATE    : 7/18/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __WINDOWS_PLATFORM_H__
#define __WINDOWS_PLATFORM_H__
#include "PlatformBase.h"
#include <windows.h>

#define USE_MULTIMEDIA_TIMER

#ifndef USE_MULTIMEDIA_TIMER
	#pragma comment(lib, "Winmm.lib")
#endif

class WindowPlatform :
	public PlatformBase
{
public:
	WindowPlatform(void);
	virtual ~WindowPlatform(void);

	virtual BOOL Create(const char* sWindowTitle, int& width, int& height);
	virtual void Destroy(void);

	virtual EGLNativeWindowType GetWindowHandle(void);
	virtual BOOL PumpMessage(BOOL bRun = TRUE);
	virtual void OnDrawComplete(void);

private:
#if 0
	struct{
		BOOL	bOveride;
		int		iWidth, iHeight;
	}m_ScreenOveride;
#endif
	BOOL		m_bInitialized;
	const char* m_sTitle;
	HWND		m_hWnd;
	MSG			m_Msg;
};

#endif//__WINDOWS_PLATFORM_H__
