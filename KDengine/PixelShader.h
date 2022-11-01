#pragma once
#include "Bindable.h"
#include "Graphics.h"

class PixelShader : public Bindable
{
protected:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;

public:
	PixelShader( Graphics& gfx, const std::wstring path );
	void Bind( Graphics& gfx ) noexcept override;
};