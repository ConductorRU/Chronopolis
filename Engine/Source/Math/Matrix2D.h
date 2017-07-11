#pragma once
namespace DEN
{
	class Vector2;
	class Vector;
	class Quaternion;
	struct Matrix4;
	class Matrix2D
	{
	public:
		float m[3][2];
		static const Matrix2D IDENTITY;
		inline Matrix2D()
		{
			m[0][0] = 1.0f; m[0][1] = 0.0f;
			m[1][0] = 0.0f; m[1][1] = 1.0f;
			m[2][0] = 0.0f; m[2][1] = 0.0f;
		}
		inline Matrix2D(float m11, float m12, float m21, float m22, float m31, float m32)
		{
			m[0][0] = m11; m[0][1] = m12;
			m[1][0] = m21; m[1][1] = m22;
			m[2][0] = m31; m[2][1] = m32;
		}
		inline Matrix2D operator * (Matrix2D &m2)
		{
			Matrix2D r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0];
			r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1];
			r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0];
			r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1];
			r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m2.m[2][0];
			r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m2.m[2][1];
			return r;
		}
		inline Matrix2D Matrix2D::Inverse() const
		{
			float t00 = m[1][1];
			float t10 = -m[0][1];
			float invDet = 1.0f / (t00 * m[0][0] + t10 * m[1][0]);
			float d00 = t00 * invDet;
			float d10 = t10 * invDet;
			float d01 = -m[1][0] * invDet;
			float d11 = m[0][0] * invDet;
			float v1 = m[0][1];
			float v2 = m[3][2] * m[0][1];
			float v3 = m[1][1];
			float v4 = m[3][2] * m[1][1];
			float v5 = m[3][2] * m[2][1] - m[3][1];
			float d03 = -(v5 * m[1][0] - v4 * m[2][0] + v3 * m[3][0]) * invDet;
			float d13 = (v5 * m[0][0] - v2 * m[2][0] + v1 * m[3][0]) * invDet;
			return Matrix2D(d00, d10, d01, d11, d03, d13);
		}
		inline float* operator [] (UINT row)
		{
			return m[row];
		}
		inline float& operator () (UINT row, UINT col)
		{
			return m[row][col];
		}
		inline float operator () (UINT row, UINT col) const
		{
			return m[row][col];
		}
		inline Matrix2D ToPositionMatrix() const
		{
			Matrix2D r = Matrix2D::IDENTITY;
			r.m[2][0] = m[2][0];
			r.m[2][1] = m[2][1];
			return r;
		}
		void SetTranslation(const Vector2 &p);
		void SetTranslationX(float x);
		void SetTranslationY(float y);
		void SetScale(const Vector2 &p);
		Vector2 GetTranslation() const;
		Vector2 GetScale() const;
		void Transformation(const Vector2& position, const Vector2& scale, const Quaternion &orient);
		Vector2 GetLook() const;
		void TransformScreen(const Vector2 &p, const Vector2 &center, Vector &dest) const;
		void TransformOffsetScreen(const Vector2 &p, const Vector2 &center, const Vector2 &offset, Vector &dest) const;
		Vector2 TransformCoord(const Vector2 &p) const;
		void RotationAxis(const Vector& axis, const float& rad);
		void RotationX(const float& rad);
		void RotationY(const float& rad);
		void RotationZ(const float& rad);
		Quaternion ToQuaternion() const;
		Matrix4 ToMatrix() const;//нужно для отображения d3dxText
	};
}