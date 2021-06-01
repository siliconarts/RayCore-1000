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
// DATE    : 11/14/2013 Thu
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#include "WindowsPlatform.h"
#include <stdio.h>
#include <WindowsX.h>

#if 0
#include "WindowSurface.h"
#include "TestDriver.h"
#include "driver/SystemConfig.h"
extern "C"{
	static int	__screen_width	= 0;
	static int	__screen_height	= 0;
	static void GetWindowSize(int& width, int& height){
		width	= __screen_width;
		height	= __screen_height;
	}
};

static EGL_WINDOW_SURFACE	__window_surface = {
	GetWindowSize
};
#endif

static PlatformBase*	g_pPlatform			= NULL;
static BOOL				g_bUseTDDisplay		= FALSE;
HINSTANCE				g_hInstance			= NULL;
static const char*		g_sWindowClassName	= "RC.Example";
DWORD					g_dwMouseButton		= 0;
float					g_fMouseX			= 0;
float					g_fMouseY			= 0;
BOOL					g_bMouseCaptured	= FALSE;

float GetElapsedTime(void){
#ifdef USE_MULTIMEDIA_TIMER
	static LONGLONG	llPrevTime;
	static BOOL		bInitialize = FALSE;
	static LONGLONG llQPFTicksPerSec  = 0;
	LONGLONG		CurTime;
	float			fElapsedTime;
	if(!bInitialize){
		QueryPerformanceFrequency((LARGE_INTEGER*)&llQPFTicksPerSec);
		bInitialize	= TRUE;		
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&CurTime);
	fElapsedTime	= (float)(CurTime - llPrevTime) / llQPFTicksPerSec;
	llPrevTime		= CurTime;
#else
	static DWORD	dwPrevTime;
	static BOOL		bInitialize = FALSE;
	DWORD			dwCurTime;
	float			fElapsedTime;
	if(!bInitialize){
		dwPrevTime	= timeGetTime();
		bInitialize	= TRUE;		
	}
	dwCurTime		= timeGetTime();
	fElapsedTime	= (float)(dwCurTime - dwPrevTime) / 1000.f;
	dwPrevTime		= dwCurTime;
#endif
	return fElapsedTime;
}

WindowPlatform::WindowPlatform(void){
	m_bInitialized	= FALSE;
	m_hWnd			= NULL;
	g_pPlatform		= this;

#if 0
	{
		ITestDriverMemory* pMemory	= TestDriver_GetMemory("RayCore");
		if(pMemory){
			SYSTEM_CONFIG* pConfig		= (SYSTEM_CONFIG*)pMemory->GetConfig();
			g_bUseTDDisplay				= pConfig->bUseTestDriveDisplay;
			m_ScreenOveride.bOveride	= pConfig->screen.bOveride;
			m_ScreenOveride.iWidth		= pConfig->screen.iWidth;
			m_ScreenOveride.iHeight		= pConfig->screen.iHeight;
			TestDriver_Cleanup();
		}else{
			m_ScreenOveride.bOveride	= FALSE;
		}
	}
#endif
}

// windows message proc.
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
		return 0;
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		break;
	case WM_ERASEBKGND:
		return 0;
	case WM_LBUTTONDOWN:
		g_dwMouseButton	|= (1<<0);
		goto UPDATE_BUTTON_STATUS;
	case WM_LBUTTONUP:
		g_dwMouseButton	&= ~(1<<0);
		goto UPDATE_BUTTON_STATUS;
	case WM_RBUTTONDOWN:
		g_dwMouseButton	|= (1<<1);
		goto UPDATE_BUTTON_STATUS;
	case WM_RBUTTONUP:
		g_dwMouseButton	&= ~(1<<1);
		goto UPDATE_BUTTON_STATUS;
	case WM_MBUTTONDOWN:
		g_dwMouseButton	|= (1<<2);
		goto UPDATE_BUTTON_STATUS;
	case WM_MBUTTONUP:
		g_dwMouseButton	&= ~(1<<2);
		goto UPDATE_BUTTON_STATUS;
	case WM_MOUSEMOVE:
		g_fMouseX	= ((float)GET_X_LPARAM(lParam)) / g_pPlatform->Width();
		g_fMouseY	= ((float)GET_Y_LPARAM(lParam)) / g_pPlatform->Height();
UPDATE_BUTTON_STATUS:
		if(g_dwMouseButton){
			if(!g_bMouseCaptured){
				SetCapture(hWnd);
				g_bMouseCaptured	= TRUE;
			}
		}else{
			if(g_bMouseCaptured){
				ReleaseCapture();
				g_bMouseCaptured	= FALSE;
			}
		}
		g_pPlatform->OnMouseEvent(g_fMouseX, g_fMouseY, g_dwMouseButton);
		break;
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case VK_ADD:		//+		: 91
				g_pPlatform->KeyDownEvent(0x5A + 1);
				break;
			case VK_SUBTRACT:	//-		: 92
				g_pPlatform->KeyDownEvent(0x5A + 2);
				break;
			case VK_UP:			//¡è		: 93
				g_pPlatform->KeyDownEvent(0x5A + 3);
				break;
			case VK_DOWN:		//¡é		: 94
				g_pPlatform->KeyDownEvent(0x5A + 4);
				break;
			case VK_LEFT:		//¡ç		: 95
				g_pPlatform->KeyDownEvent(0x5A + 5);
				break;
			case VK_RIGHT:		//¡æ		: 96
				g_pPlatform->KeyDownEvent(0x5A + 6);
				break;
			case VK_NUMPAD0:	//0
			case VK_NUMPAD1:
			case VK_NUMPAD2:
			case VK_NUMPAD3:
			case VK_NUMPAD4:
			case VK_NUMPAD5:
			case VK_NUMPAD6:
			case VK_NUMPAD7:
			case VK_NUMPAD8:
			case VK_NUMPAD9:	//9
				g_pPlatform->KeyDownEvent( 0x30 + ((char)wParam - VK_NUMPAD0) );
				break;
			default:
				//0x30 ~ 0x39 : 0~9	: 48 ~ 57
				//0x41 ~ 0x5A : A~Z : 65 ~ 90
				if( (wParam < '0' || wParam > '9') && (wParam < 'A' || wParam > 'Z') )
					break;
				g_pPlatform->KeyDownEvent((char)wParam);
				break;
			}
		}
		break;
	default:
		break;
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}

// Regist the window class
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= g_sWindowClassName;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}

// Create the main window with an instance handle
static HWND __CreateWindow(const char* sTitle, int width, int height)
{
	HWND hWnd	= NULL;

	DWORD	nWidth		= width;
	DWORD	nHeight		= height;

	{	// Recalculate the actual window's size to fit the client region in window
		RECT		sRect;
		SetRect(&sRect, 0, 0, nWidth, nHeight);
		AdjustWindowRectEx(&sRect, WS_CAPTION | WS_SYSMENU, false, 0);
		nWidth	= sRect.right - sRect.left;
		nHeight	= sRect.bottom - sRect.top;
	}
	hWnd = CreateWindow(g_sWindowClassName, sTitle, WS_VISIBLE | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, nWidth, nHeight, NULL, NULL, g_hInstance, NULL);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	{	// Áß¾Ó¿¡ 'Loading...' Ãâ·Â
		const char	sLoading[]	= "Loading...";
		HDC			hDC			= GetDC(hWnd);
		RECT		sRect;
		GetClientRect(hWnd, &sRect);
		DrawText(hDC, sLoading, -1, &sRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		ReleaseDC(hWnd, hDC);
	}

	return hWnd;
}


WindowPlatform::~WindowPlatform(void){
	g_pPlatform	= NULL;
}

BOOL WindowPlatform::Create(const char* sWindowTitle, int& width, int& height){
#if 0
	if(m_ScreenOveride.bOveride){
		width		= m_ScreenOveride.iWidth;
		height		= m_ScreenOveride.iHeight;
	}
	if(g_bUseTDDisplay){
		__screen_width		= width;
		__screen_height		= height;
	}else
#endif
	{
		g_hInstance	= GetModuleHandle(NULL);
		MyRegisterClass(g_hInstance);
		
		m_hWnd		= __CreateWindow(sWindowTitle, width, height);
		m_sTitle	= sWindowTitle;
	}
	m_bInitialized	= TRUE;
	GetElapsedTime();
	return TRUE;
}

void WindowPlatform::Destroy(void){
	if(!m_bInitialized) return;
	DestroyWindow(m_hWnd);
}

EGLNativeWindowType WindowPlatform::GetWindowHandle(void){
#if 0
	if(g_bUseTDDisplay){
		return (EGLNativeWindowType)&__window_surface;
	}
#endif
	return m_hWnd;
}

BOOL WindowPlatform::PumpMessage(BOOL bRun){
	for(;;){
		if(GetKeyState(VK_ESCAPE)<0) return FALSE;
		if(PeekMessage(&m_Msg, NULL, 0, 0, PM_REMOVE)){
			if(m_Msg.message == WM_QUIT) return FALSE;
			TranslateMessage(&m_Msg);
			DispatchMessage(&m_Msg);
		}else break;
	}

	return bRun;
}

void WindowPlatform::OnDrawComplete(void){
#if 0
	if(!g_bUseTDDisplay)
#endif
	{
		static int		render_count	= 0;
		render_count++;

		if(render_count == 10){
			char sTitle[MAX_PATH];
			sprintf(sTitle, "%s : %.2f fps", m_sTitle, 10.f / GetElapsedTime());
			CastElapsedTime(sTitle);
			SetWindowText(m_hWnd, sTitle);

			render_count	= 0;
		}
	}
}
