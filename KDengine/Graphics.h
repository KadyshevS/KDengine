#pragma once
#include "KDWin.h"
#include "KDException.h"
#include "DxgiInfoManager.h"
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace Bind
{
	class Bindable;
}

class Graphics
{
	friend class Bind::Bindable;

	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif 
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;

	bool imguiEnabled = true;

public:
	Graphics( HWND hWnd,int width,int height );
	Graphics( const Graphics& ) = delete;
	Graphics& operator = ( const Graphics& ) = delete;

	void EndFrame();
	void BeginFrame( float r, float g, float b ) noexcept;
	void ClearBuffer( float r, float g, float b ) noexcept;
	void DrawIndexed( UINT count ) noexcept(!IS_DEBUG);
	void SetProjection( DirectX::FXMMATRIX proj ) noexcept;
	DirectX::XMMATRIX GetProjection() const noexcept;

	void SetCamera( DirectX::FXMMATRIX cam ) noexcept;
	DirectX::XMMATRIX GetCamera() const noexcept;

	void EnableImgui() noexcept;
	void DisableImgui() noexcept;
	bool IsImguiEnabled() const noexcept;

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
	class InfoException : public Exception
	{
		std::string info;

	public:
		InfoException( int line, const char* file, std::vector<std::string> infoMsgs );
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetInfo() const noexcept;
	};
	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

		std::string reason;

	public:
		const char* GetType() const noexcept override;
	};
};