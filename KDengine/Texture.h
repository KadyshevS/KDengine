#pragma once
#include "Bindable.h"
#include <memory>

class Surface;

namespace Bind
{
	class Texture : public Bindable
	{
	public:
		Texture( Graphics& gfx,const std::string& path,UINT slot = 0 );
		void Bind( Graphics& gfx ) noexcept override;
		static std::shared_ptr<Texture> Resolve( Graphics& gfx,const std::string& path,UINT slot = 0u );
		static std::string GenerateUID( const std::string& path,UINT slot = 0u );
		std::string GetUID() const noexcept override;
		bool HasAlpha() const noexcept;

	private:
		static UINT CalculateNumberOfMipLevels( UINT width,UINT height ) noexcept;

	private:
		unsigned int slot;

	protected:
		bool hasAlpha = false;
		std::string path;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	};
}
