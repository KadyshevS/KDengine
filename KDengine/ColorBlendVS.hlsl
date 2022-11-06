cbuffer CBuf
{
	matrix transform;
};
struct VSOut
{
	float4 color : Color;
	float4 pos : SV_Position;
};

VSOut main( float3 pos : Position, float4 color : Color )
{
	VSOut vout;
	vout.pos = mul( float4( pos, 1.0f ), transform );
	vout.color = color;
	return vout;
}