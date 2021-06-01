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
// DATE    : 5/14/2013 Tue
// Author  : HyungKi Jeong (hkjeong@siliconarts.co.kr)
//
//=======================================================================
#include "eglWindowDirectX.h"

extern HMODULE	g_hDll;

//-----------------------------------------------------------------------------------------
// factory
static eglWindow* __CreateWindowFactoryFunction(EGLNativeWindowType win){
	if(!win || !IsWindow(win)) return NULL;
	eglWindowDirectX* pWindow	= new eglWindowDirectX(win);
	if(!pWindow->IsInitialized()){
		pWindow->Release();
		pWindow	= NULL;
	}
	return pWindow;
}

static eglWindowFactory	__CreateWindowFactory(__CreateWindowFactoryFunction, 0);	// eglWindowWindows 보다 높은 우선순위를 가진다
//-----------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
// Helper for compiling shaders with D3DX11
//--------------------------------------------------------------------------------------
HRESULT CompileShaderFromFile(TCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	// dwShaderFlags |= D3DCOMPILE_DEBUG;

	{	// compile
		ID3DBlob* pErrorBlob;
		{	// load from resource and compile
			HRSRC	hRes	= FindResource(g_hDll, szFileName, "shaders");
			HGLOBAL	hMem	= LoadResource(g_hDll, hRes);
			LPVOID	lpRes	= LockResource(hMem);
			DWORD	size	= SizeofResource(g_hDll, hRes);

			hr = D3DX11CompileFromMemory((LPCSTR)lpRes, size, NULL, NULL, NULL, szEntryPoint, szShaderModel, 
				dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);

			FreeResource(hMem);
		}

		if(FAILED(hr)){
			if(pErrorBlob){
				//OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
				pErrorBlob->Release();
			}
			return hr;
		}
		if(pErrorBlob) pErrorBlob->Release();
	}

	return S_OK;
}

// eglWindowDirectX
eglWindowDirectX::eglWindowDirectX(EGLNativeWindowType win){
	HRESULT hr = S_OK;

	m_pd3dDevice			= NULL;
	m_pImmediateContext		= NULL;
	m_pSwapChain			= NULL;
	m_pRenderTargetView		= NULL;
	m_pVertexShader			= NULL;
	m_pPixelShader			= NULL;
	m_pVertexLayout			= NULL;
	m_pVertexBuffer			= NULL;
	m_pSamLinear			= NULL;
	m_pTexture				= NULL;
	m_pShaderResourceView	= NULL;
	m_driverType			= D3D_DRIVER_TYPE_NULL;
	m_featureLevel			= D3D_FEATURE_LEVEL_11_0;
	m_bFullScreen			= FALSE;

#ifdef USE_TESTDRIVE
	m_pTDMemory_Display	= NULL;
#endif

	if(!IsWindow(win)) goto INITIALIZING_FAILED;

	m_hWnd			= win;
	GetWindowSize(m_iWidth, m_iHeight);

	{	// create device
		UINT createDeviceFlags = 0;
		//createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			//D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE( driverTypes );

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		UINT numFeatureLevels = ARRAYSIZE( featureLevels );

		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory( &sd, sizeof( sd ) );
		sd.BufferCount							= 2;
		sd.BufferDesc.Width						= m_iWidth;
		sd.BufferDesc.Height					= m_iHeight;
		sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator		= 60;
		sd.BufferDesc.RefreshRate.Denominator	= 1;
		sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow							= m_hWnd;
		sd.SampleDesc.Count						= 1;
		sd.SampleDesc.Quality					= 0;
		sd.Windowed								= TRUE;

		for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
		{
			m_driverType = driverTypes[driverTypeIndex];
			hr = D3D11CreateDeviceAndSwapChain( NULL, m_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
				D3D11_SDK_VERSION, &sd, &m_pSwapChain, &m_pd3dDevice, &m_featureLevel, &m_pImmediateContext );
			if(SUCCEEDED(hr))
				break;
		}
		if(FAILED(hr)) goto INITIALIZING_FAILED;
	}

	{	// Create a render target view
		ID3D11Texture2D* pBackBuffer = NULL;
		hr = m_pSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pBackBuffer );
		if(FAILED(hr)) goto INITIALIZING_FAILED;

		hr = m_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &m_pRenderTargetView );
		pBackBuffer->Release();
		if(FAILED(hr)) goto INITIALIZING_FAILED;

		m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	}

	{	// vertex shader
		// Compile the vertex shader
		ID3DBlob* pVSBlob = NULL;
		hr = CompileShaderFromFile("shader.fx", "VS", "vs_4_0", &pVSBlob);
		if(FAILED(hr)){
			MessageBox( NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK);
			goto INITIALIZING_FAILED;
		}
		// Create the vertex shader
		hr = m_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_pVertexShader);
		if(FAILED(hr)){	
			pVSBlob->Release();
			goto INITIALIZING_FAILED;
		}

		{	// Define the input layout
			D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			UINT numElements = ARRAYSIZE( layout );

			// Create the input layout
			hr = m_pd3dDevice->CreateInputLayout( layout, numElements, pVSBlob->GetBufferPointer(),
				pVSBlob->GetBufferSize(), &m_pVertexLayout );
			pVSBlob->Release();
			if(FAILED(hr)) goto INITIALIZING_FAILED;

			// Set the input layout
			m_pImmediateContext->IASetInputLayout(m_pVertexLayout);
		}
	}

	{	// pixel shader
		// Compile the pixel shader
		ID3DBlob* pPSBlob = NULL;
		hr = CompileShaderFromFile("shader.fx", "PS", "ps_4_0", &pPSBlob );
		if(FAILED(hr)){
			MessageBox( NULL, "The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", "Error", MB_OK );
			goto INITIALIZING_FAILED;
		}

		// Create the pixel shader
		hr = m_pd3dDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pPixelShader );
		pPSBlob->Release();
		if(FAILED(hr)) goto INITIALIZING_FAILED;
	}

	{	// Create vertex buffer
		VERTEXFORMAT_PLANE vertices[] =
		{
			XMFLOAT3( -1.0f,  1.0f, 0.5f ), XMFLOAT2(0.f,1.f),
			XMFLOAT3(  1.0f,  1.0f, 0.5f ), XMFLOAT2(1.f,1.f),
			XMFLOAT3( -1.0f, -1.0f, 0.5f ), XMFLOAT2(0.f,0.f),
			XMFLOAT3(  1.0f, -1.0f, 0.5f ), XMFLOAT2(1.f,0.f),
		};
		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof(bd) );
		bd.Usage			= D3D11_USAGE_DEFAULT;
		bd.ByteWidth		= sizeof( VERTEXFORMAT_PLANE ) * 4;
		bd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags	= 0;
		D3D11_SUBRESOURCE_DATA InitData;
		ZeroMemory( &InitData, sizeof(InitData) );
		InitData.pSysMem	= vertices;
		hr = m_pd3dDevice->CreateBuffer( &bd, &InitData, &m_pVertexBuffer );
		if(FAILED(hr)) goto INITIALIZING_FAILED;

		// Set vertex buffer
		UINT stride = sizeof( VERTEXFORMAT_PLANE );
		UINT offset = 0;
		m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		// Set primitive topology
		m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	}
	{	// create sampling layer
		// Create state objects
		D3D11_SAMPLER_DESC samDesc;
		ZeroMemory(&samDesc, sizeof(samDesc));
		samDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samDesc.AddressU		=
		samDesc.AddressV		=
		samDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
		samDesc.MaxAnisotropy	= 1;
		samDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		samDesc.MaxLOD			= D3D11_FLOAT32_MAX;
		hr = m_pd3dDevice->CreateSamplerState(&samDesc, &m_pSamLinear);
		if(FAILED(hr)) goto INITIALIZING_FAILED;
	}
	{	// create texture
		// Create depth stencil texture
/*		D3D11_SUBRESOURCE_DATA TexInitData;
		ZeroMemory(&TexInitData, sizeof(D3D11_SUBRESOURCE_DATA));
		DWORD* pBuff				= new DWORD[m_iWidth * m_iHeight];
		TexInitData.pSysMem			= pBuff;
		TexInitData.SysMemPitch		= m_iWidth * sizeof(DWORD);

		for(int y=0;y<m_iHeight;y++)
		for(int x=0;x<m_iWidth;x++){
			pBuff[x+(y*m_iWidth)]	= (((x+y)&0xFF)<<16) | ((x&0xFF)<<8) | (y&0xFF);
		}*/

		D3D11_TEXTURE2D_DESC descTex;
		ZeroMemory(&descTex, sizeof(D3D11_TEXTURE2D_DESC));
		descTex.Width				= m_iWidth;
		descTex.Height				= m_iHeight;
		descTex.MipLevels			= 1;
		descTex.ArraySize			= 1;
		descTex.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
		descTex.SampleDesc.Count	= 1;
		descTex.SampleDesc.Quality	= 0;
		descTex.Usage				= D3D11_USAGE_DYNAMIC;
		descTex.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		descTex.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		descTex.MiscFlags			= 0;
		//hr = m_pd3dDevice->CreateTexture2D( &descTex, &TexInitData, &m_pTexture);
		hr = m_pd3dDevice->CreateTexture2D( &descTex, NULL, &m_pTexture);

//		delete [] pBuff;

		if(FAILED(hr)) goto INITIALIZING_FAILED;
		
		m_pd3dDevice->CreateShaderResourceView(m_pTexture, NULL,  &m_pShaderResourceView);
	}
	
	RestoreDeviceObjects();
	
#ifdef USE_TESTDRIVE
	m_pTDMemory_Display		= TestDriver_GetMemory(DISPLAY_NAME);
	m_pDisplayConfig		= m_pTDMemory_Display ? (DisplayConfig*)m_pTDMemory_Display->GetConfig() : NULL;
#endif
	return;

INITIALIZING_FAILED:
	ReleaseAll();
}

eglWindowDirectX::~eglWindowDirectX(void){
	ReleaseAll();

#ifdef USE_TESTDRIVE
	SAFE_RELEASE(m_pTDMemory_Display);
#endif
}

void eglWindowDirectX::ReleaseAll(void){
	if(m_pImmediateContext) m_pImmediateContext->ClearState();
	SAFE_RELEASE(m_pSamLinear);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pVertexLayout);
	SAFE_RELEASE(m_pVertexShader);
	SAFE_RELEASE(m_pPixelShader);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pImmediateContext);
	SAFE_RELEASE(m_pd3dDevice);
}

void eglWindowDirectX::GetWindowSize(int &iWidth, int &iHeight){
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

BOOL eglWindowDirectX::RestoreDeviceObjects(void){
	// Setup the viewport
	if(m_pImmediateContext){
		D3D11_VIEWPORT vp;
		vp.Width		= (FLOAT)m_iWidth;
		vp.Height		= (FLOAT)m_iHeight;
		vp.MinDepth		= 0.0f;
		vp.MaxDepth		= 1.0f;
		vp.TopLeftX		= 0;
		vp.TopLeftY		= 0;
		m_pImmediateContext->RSSetViewports(1, &vp);

		if(m_pSamLinear)
			m_pImmediateContext->PSSetSamplers(0, 1, &m_pSamLinear);

		if(m_pShaderResourceView)
			m_pImmediateContext->PSSetShaderResources(0, 1, &m_pShaderResourceView);
	}
	return TRUE;
}

#ifdef USE_TESTDRIVE
#include "egl/eglSurface.h"
#endif

void eglWindowDirectX::Present(RCDDK* pDDK, DrawBuffer* pBuffer){
//	ToogleFullScreen(TRUE);

	if(m_pd3dDevice){
		// Clear the back buffer 
		float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
		//m_pImmediateContext->ClearRenderTargetView( m_pRenderTargetView, ClearColor );

		if(m_pTexture){
			Memory* pMem		= pBuffer->GetMemory();
			D3D11_MAPPED_SUBRESOURCE map;
			ZeroMemory(&map, sizeof(D3D11_MAPPED_SUBRESOURCE));
			m_pImmediateContext->Map(m_pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &map);

			if(map.RowPitch == m_iWidth*sizeof(DWORD)){
				// byte stride 가 없을 경우
				RtlCopyMemory(map.pData, pMem->Virtual(), m_iWidth*m_iHeight*sizeof(DWORD));
			}else{
				// byte stride 가 존재할 경우
				BYTE*	pDst		= (BYTE*)map.pData;
				BYTE*	pSrc		= (BYTE*)pMem->Virtual();
				DWORD	src_stride	= m_iWidth*sizeof(DWORD);

				for(int y=0;y<m_iHeight;y++){
					RtlCopyMemory(pDst, pSrc, src_stride);
					pSrc	+= src_stride;
					pDst	+= map.RowPitch;
				}
			}
			
			m_pImmediateContext->Unmap(m_pTexture, 0);
		}

		// Render a triangle
		m_pImmediateContext->VSSetShader(m_pVertexShader, NULL, 0);
		m_pImmediateContext->PSSetShader(m_pPixelShader, NULL, 0);
		m_pImmediateContext->Draw(4, 0);

		// Present the information rendered to the back buffer to the front buffer (the screen)
		m_pSwapChain->Present(m_bFullScreen ? 1 : 0, 0);

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

void eglWindowDirectX::ToogleFullScreen(BOOL bFullScreen){
	if(m_bFullScreen == bFullScreen) return;
	m_bFullScreen	= bFullScreen;
	if(m_pSwapChain) m_pSwapChain->SetFullscreenState(bFullScreen, NULL);
}
