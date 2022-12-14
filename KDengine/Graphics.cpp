#include "Graphics.h"
#include "DxErr.h"
#include "GfxExcept.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include <DirectXMath.h>
#include <sstream>

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hWnd, int width, int height)
{
//	Configuring swap chain
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HRESULT hr;

//	Create back/front buffer, swap chain, rendering context
	GFX_THROW_INFO( D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	) );

//	Present back buffer
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	GFX_THROW_INFO( pSwap->GetBuffer( 0, __uuidof(ID3D11Resource), &pBackBuffer ) );
	GFX_THROW_INFO( pDevice->CreateRenderTargetView( pBackBuffer.Get(), nullptr, &pTarget ) );

//	Create depth stencil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC dDepth = {};
	dDepth.Width = width;
	dDepth.Height = height;
	dDepth.MipLevels = 1u;
	dDepth.ArraySize = 1u;
	dDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dDepth.SampleDesc.Count = 1u;
	dDepth.SampleDesc.Quality = 0u;
	dDepth.Usage = D3D11_USAGE_DEFAULT;
	dDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	GFX_THROW_INFO( pDevice->CreateTexture2D( &dDepth, nullptr, &pDepthStencil ) );

//	Create view of depth stencil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC dDSV = {};
	dDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dDSV.Texture2D.MipSlice = 0u;

	GFX_THROW_INFO( pDevice->CreateDepthStencilView( pDepthStencil.Get(), &dDSV, &pDSV) );

//	Bind depth stencil view
	pContext->OMSetRenderTargets( 1u, pTarget.GetAddressOf(), pDSV.Get() );

//	Configure viewport
	D3D11_VIEWPORT vp;
	vp.Width = (float)width;
	vp.Height = (float)height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);

// Init Imgui Impl
	ImGui_ImplDX11_Init( pDevice.Get(),pContext.Get() );
}

void Graphics::EndFrame()
{
// imgui frame end
	if (imguiEnabled)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	HRESULT hr;
#ifndef NDEBUG
	infoManager.Set();
#endif
	
	if ( FAILED( hr = pSwap->Present(1u, 0u) ) )
	{
		if ( hr = DXGI_ERROR_DEVICE_REMOVED )
		{
			throw GFX_DEVICE_REMOVED_EXCEPT( pDevice->GetDeviceRemovedReason() );
		}
		else
		{
			throw GFX_EXCEPT( hr );
		}
	}
}
void Graphics::BeginFrame(float r, float g, float b) noexcept
{
	// imgui begin frame
	if (imguiEnabled)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(), color);
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}
void Graphics::ClearBuffer(float r, float g, float b) noexcept
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView( pTarget.Get(), color );
	pContext->ClearDepthStencilView( pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u );
}
void Graphics::DrawIndexed( UINT count ) noexcept(!IS_DEBUG)
{
	GFX_THROW_INFO_ONLY( pContext->DrawIndexed( count, 0u, 0u ) );
}
void Graphics::SetProjection(DirectX::FXMMATRIX proj) noexcept
{
	projection = proj;
}
DirectX::XMMATRIX Graphics::GetProjection() const noexcept
{
	return projection;
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) noexcept
{
	camera = cam;
}
DirectX::XMMATRIX Graphics::GetCamera() const noexcept
{
	return camera;
}

void Graphics::EnableImgui() noexcept
{
	imguiEnabled = true;
}
void Graphics::DisableImgui() noexcept
{
	imguiEnabled = false;
}
bool Graphics::IsImguiEnabled() const noexcept
{
	return imguiEnabled;
}

//	Graphics Exception
Graphics::HrException::HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs ) noexcept
	:
	Exception( line, file ),
	hr( hr )
{
	for ( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	if ( !info.empty() )
	{
		info.pop_back();
	}
}
const char* Graphics::HrException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "[Error Code] 0x" << std::hex << std::uppercase << GetErrorCode()
		<< std::dec << " (" << (unsigned long)GetErrorCode() << ") " << std::endl
		<< "[Error String] " << GetErrorString() << std::endl
		<< "[Description] " << GetErrorDescription() << std::endl;

	if ( !info.empty() )
	{
		oss << "\n[Error Info]\n" << GetErrorInfo() << std::endl << std::endl;
	}

	oss << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* Graphics::HrException::GetType() const noexcept
{
	return "KDEngine Graphics Exception";
}
HRESULT Graphics::HrException::GetErrorCode() const noexcept
{
	return hr;
}
std::string Graphics::HrException::GetErrorString() const noexcept
{
	return DXGetErrorString( hr );
}
std::string Graphics::HrException::GetErrorDescription() const noexcept
{
	char buf[512];
	DXGetErrorDescription( hr, buf, sizeof( buf ) );
	return buf;
}
std::string Graphics::HrException::GetErrorInfo() const noexcept
{
	return info;
}

const char* Graphics::DeviceRemovedException::GetType() const noexcept
{
	return "KDEngine Graphics Exception [Device Removed] (DXGI_ERROR_DEVICE_REMOVED)";
}

Graphics::InfoException::InfoException(int line, const char* file, std::vector<std::string> infoMsgs)
	:
	Exception( line, file )
{
	for ( const auto& m : infoMsgs )
	{
		info += m;
		info.push_back( '\n' );
	}
	if ( !info.empty() )
	{
		info.pop_back();
	}
}
const char* Graphics::InfoException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< "\n[Error Info]\n" << GetInfo() << std::endl << std::endl;
	oss << GetOriginString();

	whatBuffer = oss.str();
	return whatBuffer.c_str();
}
const char* Graphics::InfoException::GetType() const noexcept
{
	return "KDEngine Graphics Info Exception";
}
std::string Graphics::InfoException::GetInfo() const noexcept
{
	return info;
}
