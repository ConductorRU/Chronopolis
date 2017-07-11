#include "stdafx.h"
#include "Vector2.h"
#include "Vector.h"
#include "Quaternion.h"
#include "Matrix.h"
#include "Matrix2D.h"
namespace DEN
{
	void Matrix2D::SetTranslation(const Vector2 &p)
	{
		m[2][0] = p.x;
		m[2][1] = p.y;
	}
	void Matrix2D::SetTranslationX(float x)
	{
		m[2][0] = x;
	}
	void Matrix2D::SetTranslationY(float y)
	{
		m[2][1] = y;
	}
	void Matrix2D::SetScale(const Vector2 &p)
	{
		m[0][0] = p.x;
		m[1][1] = p.y;
	}
	Vector2 Matrix2D::GetTranslation() const
	{
		return Vector2(m[2][0], m[2][1]);
	}
	Vector2 Matrix2D::GetScale() const
	{
		return Vector2(m[0][0], m[1][1]);
	}
	void Matrix2D::Transformation(const Vector2& position, const Vector2& scale, const Quaternion &orient)
	{
		Matrix2D mRot, mScale;
		orient.ToRotationMatrix(mRot);
		mScale[0][0] = scale.x;
		mScale[1][1] = scale.y;
		*this = mScale*mRot;
		SetTranslation(position);
	}
	Vector2 Matrix2D::GetLook() const
	{
		Vector2 dir;
		dir.x = m[0][0];
		dir.y = m[1][0];
		return dir;
	}
	void Matrix2D::TransformScreen(const Vector2 &p, const Vector2 &center, Vector &dest) const
	{
		dest.x = m[0][0] * (p.x - center.x) + m[1][0] * (p.y - center.y) + m[2][0] + center.x - 0.5f;
		dest.y = m[0][1] * (p.x - center.x) + m[1][1] * (p.y - center.y) + m[2][1] + center.y - 0.5f;
	}
	void Matrix2D::TransformOffsetScreen(const Vector2 &p, const Vector2 &center, const Vector2 &offset, Vector &dest) const
	{
		dest.x = m[0][0] * (p.x - center.x) + m[1][0] * (p.y - center.y) + m[2][0] + center.x + offset.x - 0.5f;
		dest.y = m[0][1] * (p.x - center.x) + m[1][1] * (p.y - center.y) + m[2][1] + center.y + offset.y - 0.5f;
	}
	Vector2 Matrix2D::TransformCoord(const Vector2 &p) const
	{
		Vector2 n;
		n.x = (m[0][0] * p.x + m[0][1] * p.y + m[2][0]);
		n.y = (m[1][0] * p.x + m[1][1] * p.y + m[2][1]);
		return n;
	}
	void Matrix2D::RotationAxis(const Vector& axis, const float& rad)
	{
		float fCos = cos(rad);
		float fSin = sin(rad);
		float fOneMinusCos = 1.0f - fCos;
		float fXYM = axis.x*axis.y*fOneMinusCos;
		float fZSin = axis.z*fSin;
		m[0][0] = axis.x*axis.x*fOneMinusCos + fCos;
		m[1][0] = fXYM - fZSin;
		m[0][1] = fXYM + fZSin;
		m[1][1] = axis.y*axis.y*fOneMinusCos + fCos;
	}
	void Matrix2D::RotationX(const float& rad)
	{
		RotationAxis(Vector(1.0f, 0.0f, 0.0f), rad);
	}
	void Matrix2D::RotationY(const float& rad)
	{
		RotationAxis(Vector(0.0f, 1.0f, 0.0f), rad);
	}
	void Matrix2D::RotationZ(const float& rad)
	{
		RotationAxis(Vector(0.0f, 0.0f, 1.0f), rad);
	}
	Quaternion Matrix2D::ToQuaternion() const
	{
		Quaternion q;
		q.FromRotationMatrix(*this);
		return q;
	}
	Matrix4 Matrix2D::ToMatrix() const//нужно для отображения d3dxText
	{
		Matrix4 mat;
		mat.m[0][0] = m[0][0]; mat.m[0][1] = m[0][1]; mat.m[0][2] = 0.0f; mat.m[0][3] = 0.0f;
		mat.m[1][0] = m[1][0]; mat.m[1][1] = m[1][1]; mat.m[1][2] = 0.0f; mat.m[1][3] = 0.0f;
		mat.m[2][0] = 0.0f;    mat.m[2][1] = 0.0f;    mat.m[2][2] = 1.0f; mat.m[2][3] = 0.0f;
		mat.m[3][0] = m[2][0]; mat.m[3][1] = m[2][1]; mat.m[3][2] = 0.0f; mat.m[3][3] = 1.0f;
		return mat;
	}
}