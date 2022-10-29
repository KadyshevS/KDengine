#pragma once
#include "KDWin.h"
#include <d3d11.h>

class Graphics
{
	ID3D11Device* pDevice = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;

public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator = ( const Graphics& ) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b ) noexcept
	{
		const float color[4] = { r, g, b, 1.0f };
		pContext->ClearRenderTargetView( pTarget, color );
	}

	~Graphics();
};