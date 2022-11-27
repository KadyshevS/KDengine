#include "Sampler.h"
#include "GfxExcept.h"
#include "BindableCodex.h"
#include "Config.h"

namespace Bind
{
	Sampler::Sampler(Graphics& gfx)
	{
		INFOMAN(gfx);

		D3D11_SAMPLER_DESC sd = {};
#ifdef ANISOTROPIC
		sd.Filter = D3D11_FILTER_ANISOTROPIC;
#else
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
#endif
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
#ifdef ANISOTROPIC
		sd.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
#endif

#ifdef MIP_MAPPING
		sd.MipLODBias = 0.0f;
		sd.MinLOD = 0.0f;
		sd.MaxLOD = D3D11_FLOAT32_MAX;
#endif

		GFX_THROW_INFO(GetDevice(gfx)->CreateSamplerState(&sd, &pSampler));
	}

	void Sampler::Bind(Graphics& gfx) noexcept
	{
		GetContext(gfx)->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
	}
	std::shared_ptr<Sampler> Sampler::Resolve(Graphics& gfx)
	{
		return Codex::Resolve<Sampler>(gfx);
	}
	std::string Sampler::GenerateUID()
	{
		return typeid(Sampler).name();
	}
	std::string Sampler::GetUID() const noexcept
	{
		return GenerateUID();
	}
}
