#include "stdafx.h"
#include "Vector2.h"
namespace DEN
{
	void Vector2::QuadLerp(const Vector2& a, const Vector2& b, const Vector2& p, const float f)
	{
		float invF = 1.0f - f;
		if(f <= 0.0f)
			*this = a;
		else if(f >= 1.0f)
			*this = b;
		*this = a*(invF*invF) + p*(2.0f*f*invF) + b*(f*f);
	}

	void Vector2::CubicLerp(const Vector2& p_1, const Vector2& p, const Vector2& p1, const Vector2& p2, const float f)
	{
		if(f <= 0.0f)
			*this = p;
		else if(f >= 1.0f)
			*this = p1;
		Vector2 a = (p - p1)*1.5f + (p2 - p_1)*0.5f;
		Vector2 b = p*-2.5f + p1*2.0f - p2*0.5f + p_1;
		Vector2 c = (p1 - p_1)*0.5f;
		Vector2 d = p;
		*this = ((a*f + b)*f + c)*f + d;
	}

	Vector2 Vector2::TransformCoord(const Vector2 &v)
	{
		Vector2 res;
		res.x = v.x*x + v.y*y;
		res.y = v.x*y + v.y*x;
		return res;
	}
}