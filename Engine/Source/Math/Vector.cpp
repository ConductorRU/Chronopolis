#include "stdafx.h"
#include "Vector.h"
namespace DEN
{
	const Vector Vector::ONE_X = Vector(1.0f, 0.0f, 0.0f);
	const Vector Vector::ONE_Y = Vector(0.0f, 1.0f, 0.0f);
	const Vector Vector::ONE_Z = Vector(0.0f, 0.0f, 1.0f);
	const Vector Vector::MINUS_X = Vector(-1.0f, 0.0f, 0.0f);
	const Vector Vector::MINUS_Y = Vector(0.0f, -1.0f, 0.0f);
	const Vector Vector::MINUS_Z = Vector(0.0f, 0.0f, -1.0f);
	const Vector Vector::ONE = Vector(1.0f, 1.0f, 1.0f);
	const Vector Vector::ZERO = Vector(0.0f, 0.0f, 0.0f);
	void Vector::Lerp(const Vector& p1, const Vector& p2, const float f)
	{
		if(f <= 0.0f)
			*this = p1;
		else if(f >= 1.0f)
			*this = p2;
		x = p1.x + f*(p2.x - p1.x);
		y = p1.y + f*(p2.y - p1.y);
		z = p1.z + f*(p2.z - p1.z);
	}

	void Vector::QuadLerp(const Vector& a, const Vector& b, const Vector& p, const float f)
	{
		float invF = 1.0f - f;
		if(f <= 0.0f)
			*this = a;
		else if(f >= 1.0f)
			*this = b;
		*this = a*(invF*invF) + p*(2.0f*f*invF) + b*(f*f);
	}

	void Vector::CubicLerp(const Vector& p_1, const Vector& p, const Vector& p1, const Vector& p2, const float f)
	{
		if(f <= 0.0f)
			*this = p;
		else if(f >= 1.0f)
			*this = p1;
		Vector a = (p - p1)*1.5f + (p2 - p_1)*0.5f;
		Vector b = p*-2.5f + p1*2.0f - p2*0.5f + p_1;
		Vector c = (p1 - p_1)*0.5f;
		Vector d = p;
		*this = ((a*f + b)*f + c)*f + d;
	}

	void Vector::LerpRange(const Vector& p1, const Vector& p2, const float f)
	{
		if(f < 0.0f)
			*this = p1;
		else if(f > 1.0f)
			*this = p2;
		x = p1.x + f*(p2.x - p1.x);
		y = p1.y + f*(p2.y - p1.y);
		z = p1.z + f*(p2.z - p1.z);
	}

	bool Vector::IntersectTriangle(const Vector& orig, const Vector& dir, Vector& v0, Vector& v1, Vector& v2, float* dist, float* u, float* v)
	{
		Vector edge1 = v1 - v0;
		Vector edge2 = v2 - v0;
		Vector pvec;
		pvec = dir.Cross(edge2);
		float det = edge1.Dot(pvec);
		Vector tvec;
		if(det > 0)
			tvec = orig - v0;
		else
		{
			tvec = v0 - orig;
			det = -det;
		}
		if(det < 0.0001f)
			return false;
		*u = tvec.Dot(pvec);
		if(*u < 0.0f || *u > det)
			return false;
		Vector qvec = tvec.Cross(edge1);
		*v = dir.Dot(qvec);
		if(*v < 0.0f || *u + *v > det)
			return false;
		*dist = edge2.Dot(qvec);
		float fInvDet = 1.0f / det;
		*dist *= fInvDet;
		*u *= fInvDet;
		*v *= fInvDet;
		return true;
	}
}