#pragma once
#include "Bindable.h"

class Topology : public Bindable
{
protected:
	D3D11_PRIMITIVE_TOPOLOGY type;

public:
	Topology( Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type );
	void Bind( Graphics& gfx ) noexcept override;
};