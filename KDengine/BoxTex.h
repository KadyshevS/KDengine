#pragma once
#include "DrawableBase.h"
#include "IndexedTriangleList.h"
#include <random>

class BoxTex : public DrawableBase<BoxTex>
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos;
		struct
		{
			float u;
			float v;
		} tex;
	};

	BoxTex(Graphics& gfx, std::mt19937& rng,
		std::uniform_real_distribution<float>& adist,
		std::uniform_real_distribution<float>& ddist,
		std::uniform_real_distribution<float>& odist,
		std::uniform_real_distribution<float>& rdist,
		const IndexedTriangleList<BoxTex::Vertex> obj);
	void Update(float dt) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};