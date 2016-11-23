#include "stdafx.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
namespace DEN
{
	const Matrix Matrix::ZERO(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	const Matrix Matrix::IDENTITY(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
	void Matrix::RotationX(const float& rad)
	{
		RotationAxis(Vector(1.0f, 0.0f, 0.0f), rad);
	}
	void Matrix::RotationY(const float& rad)
	{
		RotationAxis(Vector(0.0f, 1.0f, 0.0f), rad);
	}
	void Matrix::RotationZ(const float& rad)
	{
		RotationAxis(Vector(0.0f, 0.0f, 1.0f), rad);
	}
	void Matrix::SetTranslation(const Vector& p)
	{
		m[0][3] = p.x;
		m[1][3] = p.y;
		m[2][3] = p.z;
	}
	void Matrix::SetScale(const Vector& p)
	{
		m[0][0] = p.x;
		m[1][1] = p.y;
		m[2][2] = p.z;
	}
	Vector Matrix::GetTranslation() const
	{
		return Vector(m[0][3], m[1][3], m[2][3]);
	}
	Vector Matrix::GetDirection() const
	{
		Vector dir;
		dir.x = m[0][0] + m[0][1] + m[0][2];
		dir.y = m[1][0] + m[1][1] + m[1][2];
		dir.z = m[2][0] + m[2][1] + m[2][2];
		return dir;
	}
	Vector Matrix::GetLook() const
	{
		Vector dir;
		dir.x = m[0][0];
		dir.y = m[0][1];
		dir.z = m[0][2];
		return dir;
	}
	Vector Matrix::GetScale() const
	{
		return Vector(m[0][0], m[1][1], m[2][2]);
	}
	Matrix Matrix::GetScaleMatrix(const Vector& p)
	{
		Matrix r;
		r.m[0][0] = p.x; r.m[0][1] = 0.0f; r.m[0][2] = 0.0f; r.m[0][3] = 0.0f;
		r.m[1][0] = 0.0f; r.m[1][1] = p.y; r.m[1][2] = 0.0f; r.m[1][3] = 0.0f;
		r.m[2][0] = 0.0f; r.m[2][1] = 0.0f; r.m[2][2] = p.z; r.m[2][3] = 0.0f;
		return r;
	}
	void Matrix::Transformation(const Vector& position, const Vector& scale, const Quaternion& orient)
	{
		Matrix mRot, mScale;
		orient.ToRotationMatrix(mRot);
		mScale[0][0] = scale.x;
		mScale[1][1] = scale.y;
		mScale[2][2] = scale.z;
		*this = mScale*mRot;
		SetTranslation(position);
	}
	void Matrix::SetOrienation(const Quaternion& orient)
	{
		Vector v = Vector(m[0][3], m[1][3], m[2][3]);
		orient.ToRotationMatrix(*this);
		SetTranslation(v);
	}
	void Matrix::Transformation(const Vector& position, const Quaternion& orient)
	{
		orient.ToRotationMatrix(*this);
		SetTranslation(position);
	}
	Matrix4 Matrix::ToMatrix() const
	{
		Matrix4 mat;
		mat.m[0][0] = m[0][0]; mat.m[0][1] = m[1][0]; mat.m[0][2] = m[2][0]; mat.m[0][3] = 0.0f;
		mat.m[1][0] = m[0][1]; mat.m[1][1] = m[1][1]; mat.m[1][2] = m[2][1]; mat.m[1][3] = 0.0f;
		mat.m[2][0] = m[0][2]; mat.m[2][1] = m[1][2]; mat.m[2][2] = m[2][2]; mat.m[2][3] = 0.0f;
		mat.m[3][0] = m[0][3]; mat.m[3][1] = m[1][3]; mat.m[3][2] = m[2][3]; mat.m[3][3] = 1.0f;
		return mat;
	}
	Vector Matrix::TransformCoord(const Vector &p) const
	{
		Vector n;
		n.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z + m[0][3]);
		n.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z + m[1][3]);
		n.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z + m[2][3]);
		return n;
	}
	Vector Matrix::TransformLook(const Vector &p) const//функция для поворота матрицы к камере
	{
		Vector n;
		n.x = (m[0][0] * p.x + m[0][1] * p.y);
		n.y = (m[1][0] * p.x + m[1][1] * p.y);
		n.z = (m[2][0] * p.x + m[2][1] * p.y);
		return n;
	}
	Vector Matrix::TransformRotation(const Vector &p) const//трансформация вершин без учёта позиции
	{
		Vector n;
		n.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z);
		n.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z);
		n.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z);
		return n;
	}
	Vector Matrix::TransformNormal(const Vector &p) const
	{
		Vector n;
		n.x = (m[0][0] * p.x + m[0][1] * p.y + m[0][2] * p.z);
		n.y = (m[1][0] * p.x + m[1][1] * p.y + m[1][2] * p.z);
		n.z = (m[2][0] * p.x + m[2][1] * p.y + m[2][2] * p.z);
		n.Normalize();//если убрать, то при изменении масштаба кости будет искажение нормали
		return n;
	}
	void Matrix::RotationAxis(const Vector& axis, const float& rad)
	{
		float fCos = cos(rad);
		float fSin = sin(rad);
		float fOneMinusCos = 1.0f - fCos;
		float fX2 = axis.x*axis.x;
		float fY2 = axis.y*axis.y;
		float fZ2 = axis.z*axis.z;
		float fXYM = axis.x*axis.y*fOneMinusCos;
		float fXZM = axis.x*axis.z*fOneMinusCos;
		float fYZM = axis.y*axis.z*fOneMinusCos;
		float fXSin = axis.x*fSin;
		float fYSin = axis.y*fSin;
		float fZSin = axis.z*fSin;
		m[0][0] = fX2*fOneMinusCos + fCos;
		m[0][1] = fXYM - fZSin;
		m[0][2] = fXZM + fYSin;
		m[1][0] = fXYM + fZSin;
		m[1][1] = fY2*fOneMinusCos + fCos;
		m[1][2] = fYZM - fXSin;
		m[2][0] = fXZM - fYSin;
		m[2][1] = fYZM + fXSin;
		m[2][2] = fZ2*fOneMinusCos + fCos;
	}
	Quaternion Matrix::ToQuaternion() const
	{
		Quaternion q;
		q.FromRotationMatrix(*this);
		return q;
	}
	Vector Matrix::TransformLookY(const Vector &p) const//функция для поворота матрицы к камере
	{
		Vector n;
		n.x = (m[0][0] * p.x);
		n.y = (m[2][2] * p.y);
		n.z = (m[1][0] * p.x + m[2][0] * p.x);
		return n;
	}

	void Matrix::LookAt(const Vector &p)
	{
		Vector right, up;
		Vector at = Vector(m[0][3], m[1][3], m[2][3]);
		Vector look = p - at;
		if(look == Vector::ZERO)
			return;
		look.Normalize();
		right = Vector::MINUS_Y.Cross(look);
		right.Normalize();
		up = look.Cross(right);
		m[2][0] = look.x;
		m[2][1] = look.y;
		m[2][2] = look.z;
		m[0][0] = right.x;
		m[0][1] = right.y;
		m[0][2] = right.z;
		m[1][0] = up.x;
		m[1][1] = up.y;
		m[1][2] = up.z;
	}

	void Matrix::RotationPitchYawRoll(float pitch, float yaw, float roll)
	{
		float fCos, fSin;
		fCos = cos(yaw);
		fSin = sin(yaw);
		//Matrix kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		//fCos = cos(pitch);
		//fSin = sin(pitch);
		//Matrix kXMat(1.0, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f);
		//fCos = cos(roll);
		//fSin = sin(roll);
		//Matrix kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0, fCos, 0.0f, 0.0f, 0.0f);
		//*this = kZMat*(kXMat*kYMat);
		Matrix kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0, fCos, 0.0f, 0.0f, 0.0f);
		fCos = cos(pitch);
		fSin = sin(pitch);
		Matrix kXMat(1.0, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f);
		fCos = cos(roll);
		fSin = sin(roll);
		Matrix kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		*this = kYMat*(kXMat*kZMat);
	}

	void Matrix::Pitch(float angle)
	{
		float fCos, fSin;
		fCos = cos(angle);
		fSin = sin(angle);
		Matrix kXMat(1.0, 0.0f, 0.0f, 0.0f, fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f);
		*this = *this*kXMat;
	}

	void Matrix::Yaw(float angle)
	{
		float fCos, fSin;
		fCos = cos(angle);
		fSin = sin(angle);
		Matrix kYMat(fCos, 0.0f, fSin, 0.0f, 1.0f, 0.0f, -fSin, 0.0, fCos, 0.0f, 0.0f, 0.0f);
		*this = kYMat**this;
	}

	void Matrix::Roll(float angle)
	{
		float fCos, fSin;
		fCos = cos(angle);
		fSin = sin(angle);
		Matrix kZMat(fCos, -fSin, 0.0f, fSin, fCos, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
		*this = *this*kZMat;
	}

	void Matrix::RotationDirection(const Vector &direction)
	{
		//RotationPitchYawRoll(min(0.0f, direction.x)*PI, direction.z*PI, direction.y*PI);
		Vector look, up, right;
		right = direction;
		right.Normalize();
		look = right.Cross(-Vector::ONE_Y);
		if(look.SquaredLength() < 0.0001f)
			look = Vector::ONE_Z;
		look.Normalize();
		up = look.Cross(-right);
		up.Normalize();
		m[0][0] = right.x;
		m[1][0] = up.x;
		m[2][0] = look.x;
		m[0][1] = right.y;
		m[1][1] = up.y;
		m[2][1] = look.y;
		m[0][2] = right.z;
		m[1][2] = up.z;
		m[2][2] = look.z;
	}

	void Matrix::RotationAxis(const Vector &ax)
	{
		float theta = -atan2f(m[2][0], m[2][2]);
		float d = ax.Length();
		float ct = cosf(theta);
		float st = sinf(theta);
		m[0][0] = ax.x*ax.x + ct*(1.0f - ax.x*ax.x);
		m[1][0] = ax.x*ax.y + ct*(-ax.x*ax.y) + st*-ax.z;
		m[2][0] = ax.x*ax.z + ct*(-ax.x*ax.z) + st*ax.y;

		m[0][1] = ax.y*ax.x + ct*(-ax.y*ax.x) + st*ax.z;
		m[1][1] = ax.y*ax.y + ct*(1 - ax.y*ax.y);
		m[2][1] = ax.y*ax.z + ct*(-ax.y*ax.z) + st*-ax.x;

		m[0][2] = ax.z*ax.x + ct*(-ax.z*ax.x) + st*-ax.y;
		m[1][2] = ax.z*ax.y + ct*(-ax.z*ax.y) + st*ax.x;
		m[2][2] = ax.z*ax.z + ct*(1 - ax.z*ax.z);
	}

	void Matrix::LookAt(const Vector &eye, const Vector &at, const Vector &up)
	{
		Vector zAxis = eye - at;//at - eye - для Left Hand Matrix
		zAxis.Normalize();
		Vector xAxis = up.Cross(zAxis);;
		xAxis.Normalize();
		Vector yAxis = zAxis.Cross(xAxis);
		m[0][0] = xAxis.x;
		m[1][0] = yAxis.x;
		m[2][0] = zAxis.x;
		m[0][1] = xAxis.y;
		m[1][1] = yAxis.y;
		m[2][1] = zAxis.y;
		m[0][2] = xAxis.z;
		m[1][2] = yAxis.z;
		m[2][2] = zAxis.z;
		m[0][3] = -xAxis.Dot(eye);
		m[1][3] = -yAxis.Dot(eye);
		m[2][3] = -zAxis.Dot(eye);
	}

	Matrix4 Matrix::Perspective(float fov, float aspect, float zNear, float zFar)
	{
		Matrix4 mat;
#ifdef OPENGL
		float y = 1.0f / tan(fov*0.5f);
		float x = y / aspect;
		float zF = zFar / (zNear - zFar);
		float zN = (zNear*zFar) / (zNear - zFar);
		mat.m[0][0] = y;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = x;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = zF;
		mat.m[2][3] = -1.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = zN;
		mat.m[3][3] = 0.0f;
#else
		float y = 1.0f / tan(fov*0.5f);
		float x = y / aspect;
		float zF = zFar / (zFar - zNear);
		float zN = (zNear*zFar) / (zNear - zFar);
		mat.m[0][0] = x;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = y;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = zF;
		mat.m[2][3] = 1.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = -zF*zNear;
		mat.m[3][3] = 0.0f;
#endif
		return mat;
	}

	Matrix4 Matrix::Orthographic(float fov, float aspect, float zNear, float zFar)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / fov;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = 2.0f / aspect;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 1.0f / (zFar - zNear);
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = 0.0f;
		mat.m[3][1] = 0.0f;
		mat.m[3][2] = zNear / (zNear - zFar);
		mat.m[3][3] = 1.0f;
		return mat;
	}

	Matrix4 Matrix::Orthographic(float minX, float maxX, float minY, float maxY, float zNear, float zFar)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / (maxX - minX);
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = 2.0f / (maxY - minY);
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 1.0f / (zFar - zNear);
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = (minX + maxX) / (minX - maxX);
		mat.m[3][1] = (maxY + minY) / (minY - maxY);
		mat.m[3][2] = zNear / (zNear - zFar);
		mat.m[3][3] = 1.0f;
		return mat;
	}

	Matrix4 Matrix::Projection2D(float width, float height)
	{
		Matrix4 mat;
		mat.m[0][0] = 2.0f / width;
		mat.m[0][1] = 0.0f;
		mat.m[0][2] = 0.0f;
		mat.m[0][3] = 0.0f;
		mat.m[1][0] = 0.0f;
		mat.m[1][1] = -2.0f / height;
		mat.m[1][2] = 0.0f;
		mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;
		mat.m[2][1] = 0.0f;
		mat.m[2][2] = 0.0f;
		mat.m[2][3] = 0.0f;
		mat.m[3][0] = -1.0f;
		mat.m[3][1] = 1.0f;
		mat.m[3][2] = 1.0f;
		mat.m[3][3] = 1.0f;
		return mat;
	}
	Matrix4 MatrixMultiply(const Matrix4 &m1, const Matrix4 &m2)
	{
		Matrix4 r;
		r.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
		r.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
		r.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
		r.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];
		r.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
		r.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
		r.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
		r.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];
		r.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
		r.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
		r.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
		r.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];
		r.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
		r.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
		r.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
		r.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
		return r;
	}

	Matrix4 MatrixInverse(const Matrix4 &m1, float *det)
	{
		Matrix4 m;
		float m00 = m1.m[0][0], m01 = m1.m[0][1], m02 = m1.m[0][2], m03 = m1.m[0][3];
		float m10 = m1.m[1][0], m11 = m1.m[1][1], m12 = m1.m[1][2], m13 = m1.m[1][3];
		float m20 = m1.m[2][0], m21 = m1.m[2][1], m22 = m1.m[2][2], m23 = m1.m[2][3];
		float m30 = m1.m[3][0], m31 = m1.m[3][1], m32 = m1.m[3][2], m33 = m1.m[3][3];
		float v0 = m20 * m31 - m21 * m30;
		float v1 = m20 * m32 - m22 * m30;
		float v2 = m20 * m33 - m23 * m30;
		float v3 = m21 * m32 - m22 * m31;
		float v4 = m21 * m33 - m23 * m31;
		float v5 = m22 * m33 - m23 * m32;
		float t00 = (v5 * m11 - v4 * m12 + v3 * m13);
		float t10 = -(v5 * m10 - v2 * m12 + v1 * m13);
		float t20 = (v4 * m10 - v2 * m11 + v0 * m13);
		float t30 = -(v3 * m10 - v1 * m11 + v0 * m12);
		float invDet = 1.0f / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
		//float invDet = 1.0f;
		m.m[0][0] = t00 * invDet;
		m.m[1][0] = t10 * invDet;
		m.m[2][0] = t20 * invDet;
		m.m[3][0] = t30 * invDet;
		m.m[0][1] = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		m.m[1][1] = (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		m.m[2][1] = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		m.m[3][1] = (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		v0 = m10 * m31 - m11 * m30;
		v1 = m10 * m32 - m12 * m30;
		v2 = m10 * m33 - m13 * m30;
		v3 = m11 * m32 - m12 * m31;
		v4 = m11 * m33 - m13 * m31;
		v5 = m12 * m33 - m13 * m32;
		m.m[0][2] = (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		m.m[1][2] = -(v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		m.m[2][2] = (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		m.m[3][2] = -(v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		v0 = m21 * m10 - m20 * m11;
		v1 = m22 * m10 - m20 * m12;
		v2 = m23 * m10 - m20 * m13;
		v3 = m22 * m11 - m21 * m12;
		v4 = m23 * m11 - m21 * m13;
		v5 = m23 * m12 - m22 * m13;
		m.m[0][3] = -(v5 * m01 - v4 * m02 + v3 * m03) * invDet;
		m.m[1][3] = (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
		m.m[2][3] = -(v4 * m00 - v2 * m01 + v0 * m03) * invDet;
		m.m[3][3] = (v3 * m00 - v1 * m01 + v0 * m02) * invDet;
		if(det)
			*det = invDet;
		return m;
	}

	/*Vector4 MatrixTransform(const Matrix4 &m, const Vector4 &p)
	{
		Vector4 n;
		float fInvW = 1.0f / (m.m[0][3] * p.x + m.m[1][3] * p.y + m.m[2][3] * p.z + m.m[3][3]);
		n.x = (m.m[0][0] * p.x + m.m[1][0] * p.y + m.m[2][0] * p.z + m.m[3][0]) * fInvW;
		n.y = (m.m[0][1] * p.x + m.m[1][1] * p.y + m.m[2][1] * p.z + m.m[3][1]) * fInvW;
		n.z = (m.m[0][2] * p.x + m.m[1][2] * p.y + m.m[2][2] * p.z + m.m[3][2]) * fInvW;
		n.w = (m.m[0][3] * p.x + m.m[1][3] * p.y + m.m[2][3] * p.z + m.m[3][3]) * fInvW;
		return n;
	}*/

	void MatrixTranspose(Matrix4 &m)
	{
		Matrix4 m1 = m;
		for(int i = 0; i != 4; ++i)
			for(int j = 0; j != 4; ++j)
				m.m[i][j] = m1.m[j][i];
	}

	void MatrixIdentity(Matrix4 &m)
	{
		m.m[0][0] = 1.0f; m.m[1][0] = 0.0f; m.m[2][0] = 0.0f; m.m[3][0] = 0.0f;
		m.m[0][1] = 0.0f; m.m[1][1] = 1.0f; m.m[2][1] = 0.0f; m.m[3][1] = 0.0f;
		m.m[0][2] = 0.0f; m.m[1][2] = 0.0f; m.m[2][2] = 1.0f; m.m[3][2] = 0.0f;
		m.m[0][3] = 0.0f; m.m[1][3] = 0.0f; m.m[2][3] = 0.0f; m.m[3][3] = 1.0f;
	}
}