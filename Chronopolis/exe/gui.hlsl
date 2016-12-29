Texture2D shaderTexture;
SamplerState SampleType;
cbuffer Properties
{
	float4 screen;
	float4 borderWidth;
	float4 borderColor;
};
struct VType
{
	float4 position : POSITION;
	float4 color : COLOR;
	float2 tex : UV;
};
struct PType
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD2;
};
PType mainVS(VType input)
{
	PType output;
	output.position.w = 1.0f;
	output.position.z = 1.0f;
	output.position.x = input.position.x*screen.x*2 - 1;
	output.position.y = -input.position.y*screen.y*2 + 1;
	output.tex = input.tex;
	output.color = input.color;
	return output;
}

float4 mainPS(PType input): SV_TARGET
{
	return input.color;
}
