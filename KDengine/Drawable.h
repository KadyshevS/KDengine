#pragma once
#include "Graphics.h"
#include "IndexBuffer.h"
#include <DirectXMath.h>

class Bindable;

class Drawable
{
	template<class T>
	friend class DrawableBase;

	const class IndexBuffer* pIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> binds;

	virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept = 0;

public:
	Drawable() = default;
	Drawable( const Drawable& ) = delete;

	virtual DirectX::XMMATRIX GetTransformXM() const noexcept = 0;
	void Draw( Graphics& gfx ) const noexcept(!IS_DEBUG);
	virtual void Update( float dt ) noexcept = 0;

	virtual ~Drawable() = default;

protected:
	template<class T>
	T* QueryBindable() noexcept
	{
		for (auto& pb : binds)
		{
			if (auto pt = dynamic_cast<T*>(pb.get()))
			{
				return pt;
			}
		}
		return nullptr;
	}
	void AddBind( std::unique_ptr<Bindable> bind ) noexcept(!IS_DEBUG);
	void AddIndexBuffer( std::unique_ptr<class IndexBuffer> ibuf ) noexcept(!IS_DEBUG);
};