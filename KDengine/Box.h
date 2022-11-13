#pragma once
#include "TestObject.h"
#include "ConstantBuffer.h"

class Box : public TestObject<Box>
{
	DirectX::XMFLOAT3X3 mt{};

	struct PSMaterialConstant
	{
		DirectX::XMFLOAT3 color = {};
		float specularIntensity = 0.6f;
		float specularPower = 30.0f;
		float padding[3] = {};
	} materialConstants;
	using MaterialCbuf = PixelConstantBuffer<PSMaterialConstant>;

	void SyncMaterial(Graphics& gfx) noexcept(!IS_DEBUG);

public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
	};

	Box(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material);

	bool SpawnControlWindow(int id, Graphics& gfx) noexcept;
};