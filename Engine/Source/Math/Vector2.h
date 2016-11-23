#pragma once
namespace DEN
{
	class Vector2
	{
	public:
		float x;
		float y;
		inline Vector2()
		{
			x = y = 0.0f;
		}
		Vector2(float pX, float pY)
		{
			x = pX;
			y = pY;
		}
		inline Vector2 operator + (const Vector2& p) const
		{
			return Vector2(x + p.x, y + p.y);
		}
		inline Vector2& operator += (const Vector2& p)
		{
			x += p.x;
			y += p.y;
			return *this;
		}
		inline bool operator== (const Vector2& p) const
		{
			return (p.x == x) && (p.y == y);
		}
		inline bool operator!= (const Vector2& p) const
		{
			return (p.x != x) || (p.y != y);
		}
		inline Vector2 operator - (const Vector2 pVector) const
		{
			return Vector2(x - pVector.x, y - pVector.y);
		}
		inline Vector2 operator - () const
		{
			return Vector2(-x, -y);
		}
		inline Vector2 operator / (const float f)
		{
			return Vector2(x / f, y / f);
		}
		inline Vector2 operator * (const float f) const
		{
			return Vector2(x*f, y*f);
		}
		inline Vector2 operator * (const Vector2 &pVector)
		{
			return Vector2(x*pVector.x, y*pVector.y);
		}
		inline Vector2 operator / (const Vector2 &pVector)
		{
			return Vector2(x / pVector.x, y / pVector.y);
		}
		inline float Length() const
		{
			return sqrt(x*x + y*y);
		}
		inline Vector2 &Abs()
		{
			x = abs(x);
			y = abs(y);
			return *this;
		}
		inline float Normalize()
		{
			float fLength = sqrt(x*x + y*y);
			if(fLength > 1e-08)
			{
				float fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
			}
			return fLength;
		}
		inline float Dot(const Vector2& vec) const
		{
			return x*vec.x + y*vec.y;
		}
		inline Vector2 Cross() const
		{
			return Vector2(-y, x);
		}
		inline float AngleBetween(const Vector2& p) const
		{
			float lenProduct = Length() * p.Length();
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;
			return acos(max(min(Dot(p) / lenProduct, 1.0f), -1.0f));
		}
		inline float GetDistance(const Vector2 &destVector) const
		{
			return sqrt((x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y));
		}
		inline float GetSquaredDistance(const Vector2 &destVector) const
		{
			return (x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y);
		}
		void QuadLerp(const Vector2& a, const Vector2& b, const Vector2& p, const float f);
		void CubicLerp(const Vector2& p_1, const Vector2& p, const Vector2& p1, const Vector2& p2, const float f);
		Vector2 TransformCoord(const Vector2 &v);
		static const Vector2 ONE;
		static const Vector2 ZERO;
	};
}