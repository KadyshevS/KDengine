#include "IndexBuffer.h"
#include "GfxExcept.h"

IndexBuffer::IndexBuffer( Graphics& gfx, std::vector<unsigned short> indicies )
	:
	count( (UINT)indicies.size() )
{
	INFOMAN(gfx);

	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof( count * sizeof( unsigned short ) );
	ibd.StructureByteStride = sizeof( unsigned short );

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indicies.data();
	GFX_THROW_INFO( GetDevice( gfx )->CreateBuffer(&ibd, &isd, &pIndexBuffer) );	
}

void IndexBuffer::Bind(Graphics& gfx) noexcept
{
	GetContext( gfx )->IASetIndexBuffer( pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u );
}
UINT IndexBuffer::GetCount() const noexcept
{
	return count;
}
