#include "BoxChkTex.h"
#include "BindableBase.h"
#include "KDMath.h"
#include "Texture.h"
#include "Surface.h"

BoxChkTex::BoxChkTex(Graphics& gfx, IndexedTriangleList<BoxChkTex::Vertex> obj, Keyboard& ctrl, const std::string& texPath)
{
	namespace dx = DirectX;
	kbd = &ctrl;

	if ( !IsStaticInitialized() )
	{
		auto model = obj;
		model.Transform(dx::XMMatrixScaling(1.5f, 1.5f, 1.5f));

		AddStaticBind(std::make_unique<Texture>(gfx, Surface::FromFile(texPath)));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"TextureVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"TexturePS.cso"));

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		struct ConstantBuffer2
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			} face_colors[6];
		};
		const ConstantBuffer2 cb2 =
		{
			{
				{ 1.0f,0.0f,1.0f },
				{ 1.0f,0.0f,0.0f },
				{ 0.0f,1.0f,0.0f },
				{ 0.0f,0.0f,1.0f },
				{ 1.0f,1.0f,0.0f },
				{ 0.0f,1.0f,1.0f },
			}
		};
		AddStaticBind(std::make_unique<PixelConstantBuffer<ConstantBuffer2>>(gfx, cb2));

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{ "Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCbuf>(gfx, *this));
}

void BoxChkTex::Update( float dt ) noexcept
{
	if (kbd->KeyIsPressed(VK_LEFT))
	{
		posX -= posXv * dt;
	}
	if (kbd->KeyIsPressed(VK_RIGHT))
	{
		posX += posXv * dt;
	}
	if (kbd->KeyIsPressed(VK_UP))
	{
		posY += posYv * dt;
	}
	if (kbd->KeyIsPressed(VK_DOWN))
	{
		posY -= posYv * dt;
	}
	if (kbd->KeyIsPressed('Z'))
	{
		posZ += posZv * dt;
	}
	if (kbd->KeyIsPressed('X'))
	{
		posZ -= posZv * dt;
	}

	if ( kbd->KeyIsPressed('Q') )
	{
		roll += rollv * dt;
		roll = wrap_angle( roll );
	}
	if ( kbd->KeyIsPressed('W') )
	{
		pitch += pitchv * dt;
		pitch = wrap_angle( pitch );
	}
	if ( kbd->KeyIsPressed('E') )
	{
		yaw += yawv * dt;
		yaw = wrap_angle( yaw );
	}
	if ( kbd->KeyIsPressed('A') )
	{
		roll -= rollv * dt;
		roll = wrap_angle( roll );
	}
	if ( kbd->KeyIsPressed('S') )
	{
		pitch -= pitchv * dt;
		pitch = wrap_angle( pitch );
	}
	if ( kbd->KeyIsPressed('D') )
	{
		yaw -= yawv * dt;
		yaw = wrap_angle( yaw );
	}
}
DirectX::XMMATRIX BoxChkTex::GetTransformXM() const noexcept
{
	return DirectX::XMMatrixRotationRollPitchYaw( pitch, yaw, roll ) *
		DirectX::XMMatrixTranslation(posX, posY, posZ+4.0f);
}
