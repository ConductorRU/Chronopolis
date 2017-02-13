#pragma once
namespace DEN
{
	class Vector2;
	class Perlin2
	{
	public:
		char *permutationTable;
		Perlin2(int seed = 0);
		Perlin2(const Perlin2 &perl, int pos);
		Vector2 GetPseudoRandomGradientVector(int x, int y);
		float QunticCurve(float t);
		float Lerp(float a, float b, float t);
		float Noise(float fx, float fy);
		float Noise(float fx, float fy, int octaves, float persistence = 0.5f);
	};
}