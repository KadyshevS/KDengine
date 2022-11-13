#pragma once
#include "Bindable.h"

namespace Bind
{
	class Sampler : public Bindable
	{
	protected:
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;

	public:
		Sampler(Graphics& gfx);
		void Bind(Graphics& gfx) noexcept override;
	};
}