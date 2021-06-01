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
#include "eglWindowWindows.h"

//-----------------------------------------------------------------------------------------
// factory
static eglWindow* __CreateWindowFactoryFunction(EGLNativeWindowType win){
	if(!win || !IsWindow(win)) return NULL;
	eglWindowWindows* pWindow	= new eglWindowWindows(win);
	if(!pWindow->IsInitialized()){
		pWindow->Release();
		pWindow	= NULL;
	}
	return pWindow;
}
static eglWindowFactory	__CreateWindowFactory(__CreateWindowFactoryFunction, 1);

//-----------------------------------------------------------------------------------------
// eglWindowWindows
eglWindowWindows::eglWindowWindows(EGLNativeWindowType win){
	m_lpDIBits	= NULL;
	m_hbmScreen	= NULL;
	m_memDC		= NULL;
	m_hDC		= NULL;
#ifdef USE_TESTDRIVE
	m_pTDMemory_Display	= NULL;
#endif
	if(!IsWindow(win)) return;

	m_hWnd	= win;
	m_hDC	= GetDC(m_hWnd);
	m_memDC	= CreateCompatibleDC(m_hDC);
	{
		GetWindowSize(m_iWidth, m_iHeight);
		memset(&m_bmpHeader, 0, sizeof(BITMAPV5HEADER));
		m_bmpHeader.bV5Size				= sizeof(BITMAPV5HEADER);
		m_bmpHeader.bV5Width			= m_iWidth;
		m_bmpHeader.bV5Height			= m_iHeight;
		m_bmpHeader.bV5Planes			= 1;
		m_bmpHeader.bV5Compression		= BI_BITFIELDS;
		// A8B8G8R8
		m_bmpHeader.bV5BitCount			= 32;
		m_bmpHeader.bV5RedMask			= 0x000000FF;
		m_bmpHeader.bV5GreenMask		= 0x0000FF00;
		m_bmpHeader.bV5BlueMask			= 0x00FF0000;
		m_bmpHeader.bV5AlphaMask		= 0xFF000000;

		m_hbmScreen = CreateDIBSection(m_memDC, (BITMAPINFO*)&m_bmpHeader, DIB_RGB_COLORS, &m_lpDIBits, NULL, 0);
		m_hbmScreen	= (HBITMAP)SelectObject(m_memDC, (HGDIOBJ)m_hbmScreen);
	}
#ifdef USE_TESTDRIVE
	m_pTDMemory_Display		= TestDriver_GetMemory(DISPLAY_NAME);
	m_pDisplayConfig		= m_pTDMemory_Display ? (DisplayConfig*)m_pTDMemory_Display->GetConfig() : NULL;
#endif
}

eglWindowWindows::~eglWindowWindows(void){
	if(m_hbmScreen){
		m_hbmScreen	= (HBITMAP)SelectObject(m_memDC, (HGDIOBJ)m_hbmScreen);
		DeleteObject((HGDIOBJ)m_hbmScreen);
	}
	if(m_memDC) DeleteDC(m_memDC);
	if(m_hDC) ReleaseDC(m_hWnd, m_hDC);

#ifdef USE_TESTDRIVE
	SAFE_RELEASE(m_pTDMemory_Display);
#endif
}

void eglWindowWindows::GetWindowSize(int &iWidth, int &iHeight){
	if(IsWindow(m_hWnd)){
		RECT rc;
		GetClientRect(m_hWnd, &rc);
		iWidth	= rc.right - rc.left;
		iHeight	= rc.bottom - rc.top;
	}else{
		iWidth	=
		iHeight	= 0;
	}
}

#ifdef USE_TESTDRIVE
#include "egl/eglSurface.h"
#endif

void eglWindowWindows::Present(RCDDK* pDDK, DrawBuffer* pBuffer){
	if(m_lpDIBits){
		DWORD dwCopyBytes	= pBuffer->Width() * pBuffer->Height() * sizeof(DWORD);
		Memory* pMem	= pBuffer->GetMemory();
		RtlCopyMemory(m_lpDIBits, pMem->Virtual(), dwCopyBytes);
		BitBlt(m_hDC, 0, 0, pBuffer->Width(), pBuffer->Height(), m_memDC, 0, 0, SRCCOPY);
		RedrawWindow(m_hWnd, NULL, NULL, RDW_UPDATENOW|RDW_INVALIDATE);

#ifdef USE_TESTDRIVE
		// 테스트 드라이브 '디스플레이' 프로파일과 연동하여 대기하도록 함
		if(m_pDisplayConfig && m_pDisplayConfig->hWndHandle && m_pDisplayConfig->bFramePause){
			m_pDisplayConfig->Front.dwAddress	= (DWORD)-1;		// 화면 갱신하지 않음.
			m_pDisplayConfig->Front.bUpdate		= TRUE;
			for(;;){
				Sleep(1);
				if(!m_pDisplayConfig->Front.bUpdate){
					if(GetKeyState(VK_SHIFT)<0){
						CURRENT_SURFACE->ClearAll();	// 덤프전 framebuffer 를 클리어시킨다.
						pDDK->DumpSystem();
					}
					break;
				}
			}
		}
#endif
	}
}
