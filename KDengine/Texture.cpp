#include "Texture.h"
#include "Surface.h"
#include "GfxExcept.h"
#include "BindableCodex.h"
#include "Config.h"

namespace Bind
{
	namespace wrl = Microsoft::WRL;

	Texture::Texture( Graphics& gfx,const std::string& path,UINT slot )
		:
		path( path ),
		slot( slot )
	{
		INFOMAN( gfx );

		// load surface
		const auto s = Surface::FromFile( path );
		hasAlpha = s.AlphaLoaded();
		
		// create texture resource
		D3D11_TEXTURE2D_DESC textureDesc = {};
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();

#ifdef MIP_MAPPING 
		textureDesc.MipLevels = 0;
#else
		textureDesc.MipLevels = 1;
#endif
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;

#ifdef MIP_MAPPING
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
#else
		textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
#endif

		textureDesc.CPUAccessFlags = 0;

#ifdef MIP_MAPPING
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
#else
		textureDesc.MiscFlags = 0;
#endif

#ifdef MIP_MAPPING
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO( GetDevice( gfx )->CreateTexture2D(
			&textureDesc,nullptr,&pTexture
		) );

		// write image data into top mip level
		GetContext( gfx )->UpdateSubresource(
			pTexture.Get(),0u,nullptr,s.GetBufferPtrConst(),s.GetWidth() * sizeof( Surface::Color ),0u
		);
#else
		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = s.GetBufferPtr();
		sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
		wrl::ComPtr<ID3D11Texture2D> pTexture;
		GFX_THROW_INFO(GetDevice(gfx)->CreateTexture2D(
			&textureDesc, &sd, &pTexture
		));
#endif

		// create the resource view on the texture
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.Format = textureDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;

#ifdef MIP_MAPPING
		srvDesc.Texture2D.MipLevels = -1;
#else
		srvDesc.Texture2D.MipLevels = 1;
#endif
		GFX_THROW_INFO( GetDevice( gfx )->CreateShaderResourceView(
			pTexture.Get(),&srvDesc,&pTextureView
		) );

#ifdef MIP_MAPPING
		// generate the mip chain using the gpu rendering pipeline
		GetContext(gfx)->GenerateMips(pTextureView.Get());
#endif
	}

	void Texture::Bind( Graphics& gfx ) noexcept
	{
		GetContext( gfx )->PSSetShaderResources( slot,1u,pTextureView.GetAddressOf() );
	}
	std::shared_ptr<Texture> Texture::Resolve( Graphics& gfx,const std::string& path,UINT slot )
	{
		return Codex::Resolve<Texture>( gfx,path,slot );
	}
	std::string Texture::GenerateUID( const std::string& path,UINT slot )
	{
		using namespace std::string_literals;
		return typeid(Texture).name() + "#"s + path + "#" + std::to_string( slot );
	}
	std::string Texture::GetUID() const noexcept
	{
		return GenerateUID( path,slot );
	}
	bool Texture::HasAlpha() const noexcept
	{
		return hasAlpha;
	}
	UINT Texture::CalculateNumberOfMipLevels( UINT width,UINT height ) noexcept
	{
		const float xSteps = std::ceil( log2( (float)width ) );
		const float ySteps = std::ceil( log2( (float)height ) );
		return (UINT)std::max( xSteps,ySteps );
	}
}
