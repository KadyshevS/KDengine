#pragma once
#include "Bindable.h"
#include <memory>

namespace Bind
{
	class Topology : public Bindable
	{
	protected:
		D3D11_PRIMITIVE_TOPOLOGY type;

	public:
		Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type);
		void Bind(Graphics& gfx) noexcept override;
		static std::shared_ptr<Topology> Resolve( Graphics& gfx,D3D11_PRIMITIVE_TOPOLOGY type );
		static std::string GenerateUID( D3D11_PRIMITIVE_TOPOLOGY type );
		std::string GetUID() const noexcept override;
	};
}