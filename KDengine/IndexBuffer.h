#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable
{
protected:
	UINT count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;

public:
	IndexBuffer( Graphics& gfx, std::vector<unsigned short> indicies );

	void Bind( Graphics& gfx ) noexcept override;
	UINT GetCount() const noexcept;
};