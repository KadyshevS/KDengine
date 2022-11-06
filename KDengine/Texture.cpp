#include "Texture.h"
#include "Surface.h"
#include "GfxExcept.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const Surface& s)
{
	INFOMAN( gfx );

	D3D11_TEXTURE2D_DESC txDesc = {};
	txDesc.Width = s.GetWidth();
	txDesc.Height = s.GetHeight();
	txDesc.MipLevels = 1u;
	txDesc.ArraySize = 1u;
	txDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	txDesc.SampleDesc.Count = 1u;
	txDesc.SampleDesc.Quality = 0u;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	txDesc.CPUAccessFlags = 0u;
	txDesc.MiscFlags = 0u;
	D3D11_SUBRESOURCE_DATA txsd = {};
	txsd.pSysMem = s.GetBufferPtr();
	txsd.SysMemPitch = s.GetWidth() * sizeof( Surface::Color );
	wrl::ComPtr<ID3D11Texture2D> pTexture;

	GFX_THROW_INFO( GetDevice( gfx )->CreateTexture2D( &txDesc, &txsd, &pTexture ) );

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = txDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0u;
	srvDesc.Texture2D.MipLevels = 1u;

	GFX_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView( pTexture.Get(), &srvDesc, &pTextureView ) );
}

void Texture::Bind(Graphics& gfx) noexcept
{
	GetContext( gfx )->PSSetShaderResources( 0u, 1u, pTextureView.GetAddressOf() );
}
