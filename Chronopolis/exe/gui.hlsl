Texture2D shaderTexture;
SamplerState SampleType;
cbuffer Properties
{
	float4 screen;
	float4x4 rect;
	float4 color;
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
	float4 borderRadius : TEXCOORD3;
};
PType mainVS(VType input)
{
	PType output;
	output.position.w = 1.0f;
	output.position.z = 1.0f;
	output.position.xy = mul(rect, input.position).xy;
	output.position.x = output.position.x*screen.x*2 - 1;
	output.position.y = -output.position.y*screen.y*2 + 1;
	output.tex = input.tex;
	output.color = input.color;
	return output;
}

float4 mainPS(PType input): SV_TARGET
{
	float4 txCol = shaderTexture.Sample(SampleType, input.tex);
	return input.color*txCol;
}
