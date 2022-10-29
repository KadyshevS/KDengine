#pragma once
#include "KDWin.h"
#include "KDException.h"
#include "DxgiInfoManager.h"
#include <d3d11.h>

class Graphics
{
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif 
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

public:
//	Graphics Exception
	class Exception : public KDException
	{
		using KDException::KDException;
	};
	class HrException : public Exception
	{
		HRESULT hr;
		std::string info;

	public:
		HrException( int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {} ) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

		std::string reason;

	public:
		const char* GetType() const noexcept override;
	};
};