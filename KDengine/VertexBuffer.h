#pragma once
#include "Bindable.h"
#include "GfxExcept.h"

class VertexBuffer : public Bindable
{
protected:
	UINT stride;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

public:
	template<typename V>
	VertexBuffer( Graphics& gfx, const std::vector<V> verticies )
		:
		stride( sizeof(V) )
	{
		INFOMAN( gfx );

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = UINT( sizeof( V ) * verticies.size() );
		bd.StructureByteStride = sizeof( V );

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = verticies.data();
		GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer(&bd, &sd, &pVertexBuffer) );
	}
	void Bind( Graphics& gfx ) noexcept override;
};