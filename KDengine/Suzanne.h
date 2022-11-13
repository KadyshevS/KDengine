#pragma once
#include "TestObject.h"
#include "ConstantBuffer.h"

class Suzanne : public TestObject<Suzanne>
{
public:
	Suzanne( Graphics& gfx,std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		DirectX::XMFLOAT3 material,
		float scale );
};