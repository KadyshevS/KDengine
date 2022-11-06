#pragma once
#include "Bindable.h"

class Texture : public Bindable
{
protected:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;

public:
	Texture( Graphics& gfx, const class Surface& s );
	void Bind( Graphics& gfx ) noexcept override;
};