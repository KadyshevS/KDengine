#pragma once
#include "DrawableBase.h"
#include "IndexedTriangleList.h"
#include "Keyboard.h"

class BoxChkTex : public DrawableBase<BoxChkTex>
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
	float posXv	= 10.0f;
	float posYv	= 10.0f;
	float posZv	= 10.0f;
//	Rotation Speed		
	float rollv	= 5.0f;
	float pitchv	= 5.0f;
	float yawv	= 5.0f;

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

	void SetRotationV( const float vel );
	void SetMoveV( const float vel );

	BoxChkTex( Graphics& gfx, IndexedTriangleList<BoxChkTex::Vertex> obj, Keyboard& ctrl, const std::string& texPath );
	
	void Update( float dt ) noexcept override;
	DirectX::XMMATRIX GetTransformXM() const noexcept override;
};