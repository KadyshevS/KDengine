#pragma once
#include "TestObject.h"
#include <array>

class Pyramid : public TestObject<Pyramid>
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		DirectX::XMFLOAT3 n;
		std::array<char, 4> color;
		char padding;
	};

	Pyramid( Graphics& gfx,std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		std::uniform_int_distribution<int>& tdist );
};