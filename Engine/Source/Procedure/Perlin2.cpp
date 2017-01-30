#include "stdafx.h"
#include "Perlin2.h"
#include "../Math/Vector2.h"
namespace DEN
{
	Perlin2::Perlin2(int seed)
	{
		srand(seed);
		permutationTable = new char[1024];
		for (int i = 0; i < 1024; ++i)
			permutationTable[i] = rand() % 256;
	}
	Vector2 Perlin2::GetPseudoRandomGradientVector(int x, int y)
	{
		int v = (int)(((x * 1836311903) ^ (y * 2971215073) + 4807526976) & 1023);
		v = permutationTable[v] & 3;

		switch (v)
		{
		case 0:  return Vector2(1.0f, 0);
		case 1:  return Vector2(-1.0f, 0);
		case 2:  return Vector2(0, 1.0f);
		default: return Vector2(0, -1.0f);
		}
	}

	float Perlin2::QunticCurve(float t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	float Perlin2::Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
	}

	float Perlin2::Noise(float fx, float fy)
	{
		int left = (int)floor(fx);
		int top = (int)floor(fy);
		float pointInQuadX = fx - left;
		float pointInQuadY = fy - top;

		Vector2 topLeftGradient = GetPseudoRandomGradientVector(left, top);
		Vector2 topRightGradient = GetPseudoRandomGradientVector(left + 1, top);
		Vector2 bottomLeftGradient = GetPseudoRandomGradientVector(left, top + 1);
		Vector2 bottomRightGradient = GetPseudoRandomGradientVector(left + 1, top + 1);

		Vector2 distanceToTopLeft = Vector2(pointInQuadX, pointInQuadY);
		Vector2 distanceToTopRight = Vector2(pointInQuadX - 1, pointInQuadY);
		Vector2 distanceToBottomLeft = Vector2(pointInQuadX, pointInQuadY - 1);
		Vector2 distanceToBottomRight = Vector2(pointInQuadX - 1, pointInQuadY - 1);

		float tx1 = distanceToTopLeft.Dot(topLeftGradient);
		float tx2 = distanceToTopRight.Dot(topRightGradient);
		float bx1 = distanceToBottomLeft.Dot(bottomLeftGradient);
		float bx2 = distanceToBottomRight.Dot(bottomRightGradient);

		pointInQuadX = QunticCurve(pointInQuadX);
		pointInQuadY = QunticCurve(pointInQuadY);

		float tx = Lerp(tx1, tx2, pointInQuadX);
		float bx = Lerp(bx1, bx2, pointInQuadX);
		float tb = Lerp(tx, bx, pointInQuadY);

		return tb;
	}

	float Perlin2::Noise(float fx, float fy, int octaves, float persistence)
	{
		float amplitude = 1;
		float max = 0;
		float result = 0;

		while (octaves-- > 0)
		{
			max += amplitude;
			result += Noise(fx, fy) * amplitude;
			amplitude *= persistence;
			fx *= 2;
			fy *= 2;
		}
		return result / max;
	}
};