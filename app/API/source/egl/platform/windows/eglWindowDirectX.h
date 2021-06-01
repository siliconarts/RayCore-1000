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
// DATE    : 5/3/2013 Fri
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================

#ifndef __EGL_WINDOW_DIRECTX_H__
#define __EGL_WINDOW_DIRECTX_H__
#include "../eglWindow.h"

#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>
#include <xnamath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxerr.lib")
#pragma comment(lib, "dxguid.lib")

#ifdef USE_TESTDRIVE
#include "TestDriver.h"
#include "driver/SystemConfig.h"
#include "VirtualDisplayConfig.h"
#endif

typedef struct{
	XMFLOAT3	pos;
	XMFLOAT2	tex;
}VERTEXFORMAT_PLANE;

class eglWindowDirectX :
	public eglWindow
{
public:
	eglWindowDirectX(EGLNativeWindowType win);
	virtual ~eglWindowDirectX(void);

	virtual void GetWindowSize(int &iWidth, int &iHeight);
	virtual void Present(RCDDK* pDDK, DrawBuffer* pBuffer);
	virtual void ToogleFullScreen(BOOL bFullScreen);

	inline BOOL IsInitialized(void)	{return (m_pd3dDevice != NULL);}

protected:
	void ReleaseAll(void);
	BOOL RestoreDeviceObjects(void);

private:
	ID3D11Device*				m_pd3dDevice;
	ID3D11DeviceContext*		m_pImmediateContext;
	IDXGISwapChain*				m_pSwapChain;
	ID3D11RenderTargetView*		m_pRenderTargetView;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11InputLayout*			m_pVertexLayout;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11SamplerState*			m_pSamLinear;
	ID3D11Texture2D*			m_pTexture;
	ID3D11ShaderResourceView*	m_pShaderResourceView;

	D3D_DRIVER_TYPE			    m_driverType;
	D3D_FEATURE_LEVEL		    m_featureLevel;

	int							m_iWidth, m_iHeight;
	HWND						m_hWnd;
	BOOL						m_bFullScreen;

#ifdef USE_TESTDRIVE
	ITestDriverMemory*			m_pTDMemory_Display;
	DisplayConfig*				m_pDisplayConfig;
#endif
};

#endif//__EGL_WINDOW_DIRECTX_H__
