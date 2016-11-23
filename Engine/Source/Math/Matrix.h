#pragma once
namespace DEN
{
	class Vector;
	class Quaternion;
	struct Matrix4
	{
		union
		{
			struct
			{
				float        _11, _12, _13, _14;
				float        _21, _22, _23, _24;
				float        _31, _32, _33, _34;
				float        _41, _42, _43, _44;
			};
			float m[4][4];
		};
		Matrix4 &Transpose()
		{
			Matrix4 mat;
			for(int i = 0; i != 4; ++i)
				for(int j = 0; j != 4; ++j)
					mat.m[i][j] = m[j][i];
			*this = mat;
			return *this;
		}
	};
	class Matrix
	{
	public:
		float m[3][4];
		void RotationX(const float& rad);
		void RotationY(const float& rad);
		void RotationZ(const float& rad);
		void SetTranslation(const Vector& p);
		void SetScale(const Vector& p);
		Vector GetTranslation() const;
		Vector GetDirection() const;
		Vector GetLook() const;
		Vector GetScale() const;
		static Matrix GetScaleMatrix(const Vector& p);
		void Transformation(const Vector& position, const Vector& scale, const Quaternion& orient);
		void SetOrienation(const Quaternion& orient);
		void Transformation(const Vector& position, const Quaternion& orient);
		Matrix4 ToMatrix() const;
		Vector TransformCoord(const Vector &p) const;
		Vector TransformLook(const Vector &p) const;
		Vector TransformRotation(const Vector &p) const;
		Vector TransformNormal(const Vector &p) const;
		void RotationAxis(const Vector& axis, const float& rad);
		Quaternion ToQuaternion() const;
		void LookAt(const Vector &p);//поворачивает ось X к точке
		Vector TransformLookY(const Vector &p) const;
		void RotationPitchYawRoll(float pitch, float yaw, float roll);
		void RotationDirection(const Vector &direction);
		void RotationAxis(const Vector &axis);
		void Pitch(float angle);
		void Yaw(float angle);
		void Roll(float angle);
		static Matrix4 Perspective(float fov, float aspect, float zNear, float zFar);
		static Matrix4 Orthographic(float fov, float aspect, float zNear, float zFar);
		static Matrix4 Orthographic(float minX, float maxX, float minY, float maxY, float zNear, float zFar);
		static Matrix4 Projection2D(float width, float height);
		void LookAt(const Vector &eye, const Vector &at, const Vector &up);//right hand
		inline Matrix()
		{
			m[0][0] = 1.0f; m[1][0] = 0.0f; m[2][0] = 0.0f;
			m[0][1] = 0.0f; m[1][1] = 1.0f; m[2][1] = 0.0f;
			m[0][2] = 0.0f; m[1][2] = 0.0f; m[2][2] = 1.0f;
			m[0][3] = 0.0f; m[1][3] = 0.0f; m[2][3] = 0.0f;
		}
		inline Matrix(float m11, float m12, float m13, float m21, float m22, float m23, float m31, float m32, float m33, float m41, float m42, float m43)
		{
			m[0][0] = m11; m[1][0] = m12; m[2][0] = m13;
			m[0][1] = m21; m[1][1] = m22; m[2][1] = m23;
			m[0][2] = m31; m[1][2] = m32; m[2][2] = m33;
			m[0][3] = m41; m[1][3] = m42; m[2][3] = m43;
		}
		inline Matrix(Matrix4 mat)
		{
			m[0][0] = mat.m[0][0]; m[1][0] = mat.m[0][1]; m[2][0] = mat.m[0][2];
			m[0][1] = mat.m[1][0]; m[1][1] = mat.m[1][1]; m[2][1] = mat.m[1][2];
			m[0][2] = mat.m[2][0]; m[1][2] = mat.m[2][1]; m[2][2] = mat.m[2][2];
			m[0][3] = mat.m[3][0]; m[1][3] = mat.m[3][1]; m[2][3] = mat.m[3][2];
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
		inline Matrix operator * (Matrix &m2)
		{
			Matrix r;
			r.m[0][0] = m[0][0] * m2.m[0][0] + m[1][0] * m2.m[0][1] + m[2][0] * m2.m[0][2];
			r.m[1][0] = m[0][0] * m2.m[1][0] + m[1][0] * m2.m[1][1] + m[2][0] * m2.m[1][2];
			r.m[2][0] = m[0][0] * m2.m[2][0] + m[1][0] * m2.m[2][1] + m[2][0] * m2.m[2][2];
			r.m[0][1] = m[0][1] * m2.m[0][0] + m[1][1] * m2.m[0][1] + m[2][1] * m2.m[0][2];
			r.m[1][1] = m[0][1] * m2.m[1][0] + m[1][1] * m2.m[1][1] + m[2][1] * m2.m[1][2];
			r.m[2][1] = m[0][1] * m2.m[2][0] + m[1][1] * m2.m[2][1] + m[2][1] * m2.m[2][2];
			r.m[0][2] = m[0][2] * m2.m[0][0] + m[1][2] * m2.m[0][1] + m[2][2] * m2.m[0][2];
			r.m[1][2] = m[0][2] * m2.m[1][0] + m[1][2] * m2.m[1][1] + m[2][2] * m2.m[1][2];
			r.m[2][2] = m[0][2] * m2.m[2][0] + m[1][2] * m2.m[2][1] + m[2][2] * m2.m[2][2];
			r.m[0][3] = m[0][3] * m2.m[0][0] + m[1][3] * m2.m[0][1] + m[2][3] * m2.m[0][2] + m2.m[0][3];
			r.m[1][3] = m[0][3] * m2.m[1][0] + m[1][3] * m2.m[1][1] + m[2][3] * m2.m[1][2] + m2.m[1][3];
			r.m[2][3] = m[0][3] * m2.m[2][0] + m[1][3] * m2.m[2][1] + m[2][3] * m2.m[2][2] + m2.m[2][3];
			return r;
		}
		inline void Multiply(Matrix &m1, Matrix &m2)
		{
			*this = m2*m1;
		}
		inline Matrix operator + (const Matrix &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] + m2.m[0][0];
			r.m[1][0] = m[1][0] + m2.m[1][0];
			r.m[2][0] = m[2][0] + m2.m[2][0];
			r.m[0][1] = m[0][1] + m2.m[0][1];
			r.m[1][1] = m[1][1] + m2.m[1][1];
			r.m[2][1] = m[2][1] + m2.m[2][1];
			r.m[0][2] = m[0][2] + m2.m[0][2];
			r.m[1][2] = m[1][2] + m2.m[1][2];
			r.m[2][2] = m[2][2] + m2.m[2][2];
			r.m[0][3] = m[0][3] + m2.m[0][3];
			r.m[1][3] = m[1][3] + m2.m[1][3];
			r.m[2][3] = m[2][3] + m2.m[2][3];
			return r;
		}
		inline Matrix operator - (const Matrix &m2) const
		{
			Matrix r;
			r.m[0][0] = m[0][0] - m2.m[0][0];
			r.m[1][0] = m[1][0] - m2.m[1][0];
			r.m[2][0] = m[2][0] - m2.m[2][0];
			r.m[0][1] = m[0][1] - m2.m[0][1];
			r.m[1][1] = m[1][1] - m2.m[1][1];
			r.m[2][1] = m[2][1] - m2.m[2][1];
			r.m[0][2] = m[0][2] - m2.m[0][2];
			r.m[1][2] = m[1][2] - m2.m[1][2];
			r.m[2][2] = m[2][2] - m2.m[2][2];
			r.m[0][3] = m[0][3] - m2.m[0][3];
			r.m[1][3] = m[1][3] - m2.m[1][3];
			r.m[2][3] = m[2][3] - m2.m[2][3];
			return r;
		}
		inline bool operator == (const Matrix& m2) const
		{
			if(m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3])
				return false;
			return true;
		}
		inline bool operator != (const Matrix& m2) const
		{
			if(m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
				m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
				m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3])
				return true;
			return false;
		}
		inline Matrix Matrix::Inverse() const
		{
			float t00 = (m[2][2] * m[1][1] - m[2][1] * m[1][2]);
			float t10 = -(m[2][2] * m[1][0] - m[2][0] * m[1][2]);
			float t20 = (m[2][1] * m[1][0] - m[2][0] * m[1][1]);
			float invDet = 1.0f / (t00 * m[0][0] + t10 * m[0][1] + t20 * m[0][2]);
			float d00 = t00 * invDet;
			float d10 = t10 * invDet;
			float d20 = t20 * invDet;
			float d01 = -(m[2][2] * m[0][1] - m[2][1] * m[0][2]) * invDet;
			float d11 = (m[2][2] * m[0][0] - m[2][0] * m[0][2]) * invDet;
			float d21 = -(m[2][1] * m[0][0] - m[2][0] * m[0][1]) * invDet;
			float d02 = (m[1][2] * m[0][1] - m[1][1] * m[0][2]) * invDet;
			float d12 = -(m[1][2] * m[0][0] - m[1][0] * m[0][2]) * invDet;
			float d22 = (m[1][1] * m[0][0] - m[1][0] * m[0][1]) * invDet;
			float v0 = m[2][1] * m[1][0] - m[2][0] * m[1][1];
			float v1 = m[2][2] * m[1][0] - m[2][0] * m[1][2];
			float v2 = m[2][3] * m[1][0] - m[2][0] * m[1][3];
			float v3 = m[2][2] * m[1][1] - m[2][1] * m[1][2];
			float v4 = m[2][3] * m[1][1] - m[2][1] * m[1][3];
			float v5 = m[2][3] * m[1][2] - m[2][2] * m[1][3];
			float d03 = -(v5 * m[0][1] - v4 * m[0][2] + v3 * m[0][3]) * invDet;
			float d13 = (v5 * m[0][0] - v2 * m[0][2] + v1 * m[0][3]) * invDet;
			float d23 = -(v4 * m[0][0] - v2 * m[0][1] + v0 * m[0][3]) * invDet;
			return Matrix(d00, d10, d20, d01, d11, d21, d02, d12, d22, d03, d13, d23);
		}
		static const Matrix ZERO;
		static const Matrix IDENTITY;
		inline Matrix ToRotationMatrix() const
		{
			Matrix r = *this;
			r.m[0][3] = 0.0f;
			r.m[1][3] = 0.0f;
			r.m[2][3] = 0.0f;
			return r;
		}
		inline Matrix ToPositionMatrix() const
		{
			Matrix r = Matrix::IDENTITY;
			r.m[0][3] = m[0][3];
			r.m[1][3] = m[1][3];
			r.m[2][3] = m[2][3];
			return r;
		}
		inline void TransformScreen(float x, float y, float &destX, float &destY) const
		{
			destX = m[0][0] * x + m[0][1] * y + m[0][3] - 0.5f;
			destY = m[1][0] * x + m[1][1] * y + m[1][3] - 0.5f;
		}
	};
	Matrix4 MatrixMultiply(const Matrix4 &m1, const Matrix4 &m2);
	Matrix4 MatrixInverse(const Matrix4 &m1, float *det = 0);
	//Vector4 MatrixTransform(const Matrix4 &m, const Vector4 &p);
	void MatrixTranspose(Matrix4 &m);
	void MatrixIdentity(Matrix4 &m);
}