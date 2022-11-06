#pragma once
#include "DrawableBase.h"
#include "IndexedTriangleList.h"
#include "Keyboard.h"

class Sheet : public DrawableBase<Sheet>
{
	Keyboard* kbd;

	//	Position
	float posX = 0.0f;
	float posY = 0.0f;
	float posZ = 0.0f;
	//	Rotation
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;

	//	Position Speed
	const float posXv = 10.0f;
	const float posYv = 10.0f;
	const float posZv = 10.0f;
	//	Rotation Speed		
	const float rollv = 5.0f;
	const float pitchv = 5.0f;
	const float yawv = 5.0f;

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

	Sheet( Graphics& gfx, Keyboard& ctrl );

	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};