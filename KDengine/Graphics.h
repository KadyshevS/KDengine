#pragma once
#include "KDWin.h"
#include "KDException.h"
#include "DxgiInfoManager.h"
#include <d3d11.h>
#include <wrl.h>

namespace wrl = Microsoft::WRL;

class Graphics
{
#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif 
	wrl::ComPtr<ID3D11Device> pDevice;
	wrl::ComPtr<IDXGISwapChain> pSwap;
	wrl::ComPtr<ID3D11DeviceContext> pContext;
	wrl::ComPtr<ID3D11RenderTargetView> pTarget;

public:
	Graphics( HWND hWnd );
	Graphics( const Graphics& ) = delete;
	Graphics& operator = ( const Graphics& ) = delete;

	void EndFrame();
	void ClearBuffer(float r, float g, float b) noexcept;

	~Graphics() = default;

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