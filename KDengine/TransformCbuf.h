#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCbuf : public Bindable
{
	VertexConstantBuffer<DirectX::XMMATRIX> vcbuf;
	const Drawable& parent;

public:
	TransformCbuf( Graphics& gfx, const Drawable& parent );
	void Bind( Graphics& gfx ) noexcept override;
};