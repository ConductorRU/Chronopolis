Texture2D shaderTexture;
SamplerState SampleType;
cbuffer Properties
{
	float4 screen;
	float4 borderWidth;
	float4 borderColor;
	float4 borderRadius;
	float4 rect;
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
	output.position.x = input.position.x*screen.x*2 - 1;
	output.position.y = -input.position.y*screen.y*2 + 1;
	output.tex = input.tex;
	output.color = input.color;
	return output;
}

float4 mainPS(PType input): SV_TARGET
{
	float4 txCol = shaderTexture.Sample(SampleType, input.tex);
	float2 pnt = float2(input.position.x, input.position.y);
	float4 rec = float4(rect.x + borderRadius.x, rect.y + borderRadius.x, rect.z - input.position.x, rect.a - input.position.y);
	if(borderRadius.x > 0.0 && input.position.x <= rec.x && input.position.y <= rec.y)
	{
		float d = distance(pnt, float2(rect.x + borderRadius.x, rect.y + borderRadius.x));
		if(rect.x + borderRadius.x - d < 0.0)
			discard;
		if(rect.x + borderRadius.x - d < 1.0)
			txCol.a *= (rect.x + borderRadius.x - d);
	}
	if(borderRadius.y > 0.0 && rec.z <= borderRadius.y && rec.y <= borderRadius.y)
	{
		float d = distance(pnt, float2(rect.z - borderRadius.y, rect.y + borderRadius.y));
		if(borderRadius.y - d < 0.0)
			discard;
		if(borderRadius.y - d < 1.0)
			txCol.a *= (borderRadius.y - d);
	}
	if(borderRadius.z > 0.0 && rec.z <= borderRadius.z && rec.a <= borderRadius.z)
	{
		float d = distance(pnt, float2(rect.z - borderRadius.z, rect.a - borderRadius.z));
		if(borderRadius.z - d < 0.0)
			discard;
		if(borderRadius.z - d < 1.0)
			txCol.a *= (borderRadius.z - d);
	}
	if(borderRadius.a > 0.0 && rec.x <= borderRadius.a && rec.y <= borderRadius.a)
	{
		float d = distance(pnt, float2(rect.x + borderRadius.a, rect.a - borderRadius.a));
		if(borderRadius.a - d < 0.0)
			discard;
		if(borderRadius.a - d < 1.0)
			txCol.a *= (borderRadius.a - d);
	}
	return input.color*txCol;
}
