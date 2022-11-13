#pragma once
#include "Graphics.h"
#include <DirectXMath.h>
#include <memory>

namespace Bind
{
	class Bindable;
	class IndexBuffer;
}

class Drawable
{
	template<class T>
	friend class DrawableBase;
public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;
	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
	virtual void Update( float dt ) noexcept
	{}
	virtual ~Drawable() = default;
protected:
	template<class T>
	T* QueryBindable() noexcept
	{
		for( auto& pb : binds )
		{
			if( auto pt = dynamic_cast<T*>(pb.get()) )
			{
				return pt;
			}
		}
		return nullptr;
	}
	void AddBind( std::unique_ptr<Bind::Bindable> bind ) noexcept(!IS_DEBUG);
	void AddIndexBuffer( std::unique_ptr<Bind::IndexBuffer> ibuf ) noexcept(!IS_DEBUG);
private:
	virtual const std::vector<std::unique_ptr<Bind::Bindable>>& GetStaticBinds() const noexcept = 0;
private:
	const Bind::IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bind::Bindable>> binds;
};