#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>
#include "Technique.h"

class TechniqueProbe;

namespace Bind
{
	class IndexBuffer;
	class VertexBuffer;
	class Topology;
	class InputLayout;
}

class Drawable
{
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	void AddTechnique( Technique tech_in ) noexcept;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Submit( class FrameCommander& frame ) const noexcept;
	void Bind( Graphics& gfx ) const noexcept;
	void Accept( TechniqueProbe& probe );
	UINT GetIndexCount() const noexcept(!IS_DEBUG);
	virtual ~Drawable();

protected:
	std::shared_ptr<Bind::IndexBuffer> pIndices;
	std::shared_ptr<Bind::VertexBuffer> pVertices;
	std::shared_ptr<Bind::Topology> pTopology;
	std::vector<Technique> techniques;
};