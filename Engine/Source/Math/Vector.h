#pragma once
namespace DEN
{
	class Vector
	{
	public:
		float x;
		float y;
		float z;
		Vector()
		{
			x = y = z = 0.0f;
		}
		Vector(float p)
		{
			x = y = z = p;
		}
		Vector(float pX, float pY, float pZ)
		{
			x = pX;
			y = pY;
			z = pZ;
		}
		float GetDistance(const Vector &v) const
		{
			return sqrt((x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z));
		}
		float GetSquaredDistance(const Vector &v) const
		{
			return (x - v.x)*(x - v.x) + (y - v.y)*(y - v.y) + (z - v.z)*(z - v.z);
		}
		Vector operator + (const Vector& p) const
		{
			return Vector(x + p.x, y + p.y, z + p.z);
		}
		Vector& operator += (const Vector& p)
		{
			x += p.x;
			y += p.y;
			z += p.z;
			return *this;
		}
		Vector& operator -= (const Vector& p)
		{
			x -= p.x;
			y -= p.y;
			z -= p.z;
			return *this;
		}
		inline bool operator== (const Vector& p) const
		{
			return (p.x == x) && (p.y == y) && (p.z == z);
		}
		inline bool operator!= (const Vector& p) const
		{
			return (p.x != x) || (p.y != y) || (p.z != z);
		}
		inline Vector operator - (const Vector pVector) const
		{
			return Vector(x - pVector.x, y - pVector.y, z - pVector.z);
		}
		inline Vector operator - () const
		{
			return Vector(-x, -y, -z);
		}
		inline Vector operator / (const float f) const
		{
			return Vector(x / f, y / f, z / f);
		}
		inline Vector operator * (const float f) const
		{
			return Vector(x*f, y*f, z*f);
		}
		inline Vector operator * (const Vector &pVector) const
		{
			return Vector(x*pVector.x, y*pVector.y, z*pVector.z);
		}
		inline Vector& operator *= (const Vector& p)
		{
			x *= p.x;
			y *= p.y;
			z *= p.z;
			return *this;
		}
		inline Vector& operator /= (const Vector& p)
		{
			x /= p.x;
			y /= p.y;
			z /= p.z;
			return *this;
		}
		inline Vector operator / (const Vector &pVector) const
		{
			return Vector(x / pVector.x, y / pVector.y, z / pVector.z);
		}
		inline bool operator < (const Vector& vector) const
		{
			if(x < vector.x) return true;
			if(x == vector.x && y < vector.y) return true;
			if(x == vector.x && y == vector.y && z < vector.z) return true;
			return false;
		}
		inline bool operator > (const Vector& vector) const
		{
			if(x > vector.x) return true;
			if(x == vector.x && y > vector.y) return true;
			if(x == vector.x && y == vector.y && z > vector.z) return true;
			return false;
		}
		inline float Dot(const Vector& vec) const
		{
			return x*vec.x + y*vec.y + z*vec.z;
		}
		inline Vector Cross(const Vector& p) const
		{
			return Vector(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
		}
		inline float Length() const
		{
			return sqrt(x*x + y*y + z*z);
		}
		inline Vector &Abs()
		{
			x = abs(x);
			y = abs(y);
			z = abs(z);
			return *this;
		}
		inline float AngleBetween(const Vector& p) const
		{
			float lenProduct = Length() * p.Length();
			if(lenProduct < 1e-6f)
				lenProduct = 1e-6f;
			return acos(max(min(Dot(p) / lenProduct, 1.0f), -1.0f));
		}
		inline float Normalize()
		{
			float fLength = sqrt(x*x + y*y + z*z);
			if(fLength > 1e-08)
			{
				float fInvLength = 1.0f / fLength;
				x *= fInvLength;
				y *= fInvLength;
				z *= fInvLength;
			}
			return fLength;
		}
		inline float SquaredLength() const
		{
			return x*x + y*y + z*z;
		}
		inline Vector Perpendicular(void) const
		{
			static const float fSquareZero = (float)(1e-06 * 1e-06);
			Vector perp = Cross(Vector::ONE_X);
			if(perp.SquaredLength() < fSquareZero)
				perp = Cross(Vector::ONE_Y);
			perp.Normalize();
			return perp;
		}
		inline float Normal(const Vector& x1, const Vector& y1, const Vector& z1)
		{
			*this = (y1 - z1).Cross(x1 - y1);
			return Normalize();
		}
		inline static bool Orientation(const Vector& p0, const Vector& p1, const Vector& p2)
		{
			Vector a = p1 - p0;
			Vector b = p2 - p0;
			float sa = a.x*b.z - b.x*a.z;
			if(sa >= 0.0f)
				return true;
			return false;
		}
		inline Vector Min(const Vector &V1, const Vector &V2)
		{
			x = (V1.x < V2.x) ? V1.x : V2.x;
			y = (V1.y < V2.y) ? V1.y : V2.y;
			z = (V1.z < V2.z) ? V1.z : V2.z;
			return *this;
		}
		inline Vector Max(const Vector &V1, const Vector &V2)
		{
			x = (V1.x > V2.x) ? V1.x : V2.x;
			y = (V1.y > V2.y) ? V1.y : V2.y;
			z = (V1.z > V2.z) ? V1.z : V2.z;
			return *this;
		}
		inline Vector Floor(const Vector &V1)
		{
			x = floorf(V1.x);
			y = floorf(V1.y);
			z = floorf(V1.z);
			return *this;
		}
		inline bool Intersect(const Vector &pos, const Vector &dir, const Vector &v0, const Vector &v1, const Vector &v2, float &dist)
		{
			Vector edge1 = v1 - v0;
			Vector edge2 = v2 - v0;
			Vector pvec;
			pvec = dir.Cross(edge2);
			float det = edge1.Dot(pvec);
			Vector tvec;
			if (det > 0)
			{
				tvec = pos - v0;
			}
			else
			{
				tvec = v0 - pos;
				det = -det;
			}
			if (det < 0.0001f)
				return false;
			float u = tvec.Dot(pvec);
			if (u < 0.0f || u > det)
				return false;
			Vector qvec;
			qvec = tvec.Cross(edge1);

			float v = dir.Dot(qvec);
			if (v < 0.0f || u + v > det)
				return false;

			float t = edge2.Dot(qvec);
			float fInvDet = 1.0f / det;
			t *= fInvDet;
			u *= fInvDet;
			v *= fInvDet;
			*this = v0 * (1.0f - u - v) + v1 * u + v2 * v;
			return true;
		}
		void Lerp(const Vector& p1, const Vector& p2, const float f);
		void QuadLerp(const Vector& p_1, const Vector& p, const Vector& p1, const float f);
		void CubicLerp(const Vector& p_1, const Vector& p, const Vector& p1, const Vector& p2, const float f);
		void LerpRange(const Vector& p1, const Vector& p2, const float f);
		static bool IntersectTriangle(const Vector& orig, const Vector& dir, Vector& v0, Vector& v1, Vector& v2, float* dist, float* u, float* v);
		static const Vector ONE_X;
		static const Vector ONE_Y;
		static const Vector ONE_Z;
		static const Vector MINUS_X;
		static const Vector MINUS_Y;
		static const Vector MINUS_Z;
		static const Vector ONE;
		static const Vector ZERO;
	};
}