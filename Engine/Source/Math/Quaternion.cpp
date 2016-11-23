#include "stdafx.h"
#include "Func.h"
#include "Vector.h"
#include "Matrix.h"
#include "Matrix2D.h"
#include "Quaternion.h"
namespace DEN
{
	const Quaternion Quaternion::ZERO = Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
	const Quaternion Quaternion::IDENTITY = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
	Quaternion::Quaternion(float rad, const Vector& axis)
	{
		float fHalfAngle = 0.5f*rad;
		float fsin = sin(fHalfAngle);
		w = cos(fHalfAngle);
		x = fsin*axis.x;
		y = fsin*axis.y;
		z = fsin*axis.z;
	}
	Quaternion::Quaternion(float radX, float radY, const Vector& axis)
	{
		float f = 1.0f;
		if(radY < 0)
			f = -1.0f;
		float an = acos(radX)*f;
		float fHalfAngle = 0.5f*an;
		float fsin = sin(fHalfAngle);
		w = cos(fHalfAngle);
		x = fsin*axis.x;
		y = fsin*axis.y;
		z = fsin*axis.z;
	}
	Quaternion Quaternion::operator+ (const Quaternion& quat) const
	{
		return Quaternion(x + quat.x, y + quat.y, z + quat.z, w + quat.w);
	}

	Quaternion Quaternion::operator- (const Quaternion& quat) const
	{
		return Quaternion(x - quat.x, y - quat.y, z - quat.z, w - quat.w);
	}

	Quaternion Quaternion::operator* (const Quaternion& quat) const
	{
		return Quaternion
		(
			w*quat.x + x*quat.w + y*quat.z - z*quat.y,
			w*quat.y + y*quat.w + z*quat.x - x*quat.z,
			w*quat.z + z*quat.w + x*quat.y - y*quat.x,
			w*quat.w - x*quat.x - y*quat.y - z*quat.z
		);
	}

	Quaternion Quaternion::operator* (float f) const
	{
		return Quaternion(f*x, f*y, f*z, f*w);
	}

	Vector Quaternion::operator* (const Vector& p) const
	{
		Vector uv, uuv;
		Vector qvec(x, y, z);
		uv = qvec.Cross(p);
		uuv = qvec.Cross(uv);
		uv = uv * (2.0f * w);
		uuv = uuv * 2.0f;
		return p + uv + uuv;
	}

	Quaternion Quaternion::operator- () const
	{
		return Quaternion(-x, -y, -z, -w);
	}

	Quaternion Quaternion::Inverse() const
	{
		float fNorm = w*w + x*x + y*y + z*z;
		if(fNorm > 0.0f)
		{
			float fInvNorm = 1.0f / fNorm;
			return Quaternion(-x*fInvNorm, -y*fInvNorm, -z*fInvNorm, w*fInvNorm);
		}
		else
			return ZERO;
	}

	Vector Quaternion::ToVector()
	{
		float fTx = 2.0f*x;
		float fTy = 2.0f*y;
		float fTz = 2.0f*z;
		float fTwx = fTx*w;
		float fTxx = fTx*x;
		float fTwz = fTz*w;
		float fTxy = fTy*x;
		float fTwy = fTy*w;
		float fTyy = fTy*y;
		float fTxz = fTz*x;
		float fTyz = fTz*y;
		float fTzz = fTz*z;
		return Vector(atan2(fTyz + fTwx, 1.0f - (fTxx + fTzz)), atan2(fTxz + fTwy, 1.0f - (fTxx + fTyy)), atan2(fTxy + fTwz, 1.0f - (fTyy + fTzz)));
	}

	void Quaternion::ToAngleAxis(float& angle, Vector& axis) const
	{
		float fSqrLength = x*x + y*y + z*z;
		if(fSqrLength > 0.0f)
		{
			angle = 2.0f*acos(w);
			float fInvLength = sqrt(fSqrLength);
			axis.x = x*fInvLength;
			axis.y = y*fInvLength;
			axis.z = z*fInvLength;
		}
		else
		{
			angle = 0.0f;
			axis.x = 1.0f;
			axis.y = 0.0f;
			axis.z = 0.0f;
		}
	}

	void Quaternion::FromAngleAxis(const float& rfAngle, const Vector& rkAxis)
	{
		float fHalfAngle(0.5f*rfAngle);
		float fSin = sin(fHalfAngle);
		w = cos(fHalfAngle);
		x = fSin*rkAxis.x;
		y = fSin*rkAxis.y;
		z = fSin*rkAxis.z;
	}

	void Quaternion::FromRotationMatrix(const Matrix& kRot)
	{
		float fTrace = kRot(0, 0) + kRot(1, 1) + kRot(2, 2);
		float fRoot;
		if(fTrace > 0.0f)
		{
			fRoot = sqrt(fTrace + 1.0f);
			w = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			x = (kRot(2, 1) - kRot(1, 2))*fRoot;
			y = (kRot(0, 2) - kRot(2, 0))*fRoot;
			z = (kRot(1, 0) - kRot(0, 1))*fRoot;
		}
		else
		{
			static UINT s_iNext[3] = {1, 2, 0};
			UINT i = 0;
			if(kRot(1, 1) > kRot(0, 0))
				i = 1;
			if(kRot(2, 2) > kRot(i, i))
				i = 2;
			UINT j = s_iNext[i];
			UINT k = s_iNext[j];
			fRoot = sqrt(kRot(i, i) - kRot(j, j) - kRot(k, k) + 1.0f);
			float* apkQuat[3] = {&x, &y, &z};
			*apkQuat[i] = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			w = (kRot(k, j) - kRot(j, k))*fRoot;
			*apkQuat[j] = (kRot(j, i) + kRot(i, j))*fRoot;
			*apkQuat[k] = (kRot(k, i) + kRot(i, k))*fRoot;
		}
	}

	void Quaternion::FromRotationMatrix(const Matrix2D& kRot)
	{
		float fTrace = kRot(0, 0) + kRot(1, 1);
		float fRoot;
		if(fTrace > 0.0f)
		{
			fRoot = sqrt(fTrace + 1.0f);
			w = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			x = 0.0f;
			y = 0.0f;
			z = (kRot(0, 1) - kRot(1, 0))*fRoot;
		}
		else
		{
			static UINT s_iNext[2] = {1, 0};
			UINT i = 0;
			if(kRot(1, 1) > kRot(0, 0))
				i = 1;
			UINT j = s_iNext[i];
			fRoot = sqrt(kRot(i, i) - kRot(j, j) + 1.0f);
			float* apkQuat[3] = {&x, &y, &z};
			x = 0.5f*fRoot;
			fRoot = 0.5f / fRoot;
			w = (kRot(j, i) - kRot(i, j))*fRoot;
		}
	}

	void Quaternion::FromAxes(const Vector& xaxis, const Vector& yaxis, const Vector& zaxis)
	{
		Matrix kRot;
		kRot(0, 0) = xaxis.x;
		kRot(1, 0) = xaxis.y;
		kRot(2, 0) = xaxis.z;
		kRot(0, 1) = yaxis.x;
		kRot(1, 1) = yaxis.y;
		kRot(2, 1) = yaxis.z;
		kRot(0, 2) = zaxis.x;
		kRot(1, 2) = zaxis.y;
		kRot(2, 2) = zaxis.z;
		FromRotationMatrix(kRot);
	}

	void Quaternion::ToAxes(Vector& xAxis, Vector& yAxis, Vector& zAxis) const
	{
		Matrix kRot;
		ToRotationMatrix(kRot);
		xAxis.x = kRot(0, 0);
		xAxis.y = kRot(1, 0);
		xAxis.z = kRot(2, 0);
		yAxis.x = kRot(0, 1);
		yAxis.y = kRot(1, 1);
		yAxis.z = kRot(2, 1);
		zAxis.x = kRot(0, 2);
		zAxis.y = kRot(1, 2);
		zAxis.z = kRot(2, 2);
	}

	float Quaternion::Normalize(void)
	{
		float len = w*w + x*x + y*y + z*z;
		float factor = 1.0f / sqrt(len);
		*this = *this * factor;
		return len;
	}

	void Quaternion::ToRotationMatrix(Matrix& kRot) const
	{
		float fTx = 2.0f*x;
		float fTy = 2.0f*y;
		float fTz = 2.0f*z;
		float fTwx = fTx*w;
		float fTwy = fTy*w;
		float fTwz = fTz*w;
		float fTxx = fTx*x;
		float fTxy = fTy*x;
		float fTxz = fTz*x;
		float fTyy = fTy*y;
		float fTyz = fTz*y;
		float fTzz = fTz*z;
		kRot(0, 0) = 1.0f - (fTyy + fTzz);
		kRot(0, 1) = fTxy - fTwz;
		kRot(0, 2) = fTxz + fTwy;
		kRot(1, 0) = fTxy + fTwz;
		kRot(1, 1) = 1.0f - (fTxx + fTzz);
		kRot(1, 2) = fTyz - fTwx;
		kRot(2, 0) = fTxz - fTwy;
		kRot(2, 1) = fTyz + fTwx;
		kRot(2, 2) = 1.0f - (fTxx + fTyy);
	}

	void Quaternion::ToRotationMatrix(Matrix2D& kRot) const
	{
		float fTx = 2.0f*x;
		float fTy = 2.0f*y;
		float fTz = 2.0f*z;
		float fTwz = fTz*w;
		float fTxx = fTx*x;
		float fTxy = fTy*x;
		float fTyy = fTy*y;
		float fTzz = fTz*z;
		kRot(0, 0) = 1.0f - (fTyy + fTzz);
		kRot(1, 0) = fTxy - fTwz;
		kRot(0, 1) = fTxy + fTwz;
		kRot(1, 1) = 1.0f - (fTxx + fTzz);
	}

	float Quaternion::Dot(const Quaternion& q) const
	{
		return w*q.w + x*q.x + y*q.y + z*q.z;
	}

	void Quaternion::Nlerp(const Quaternion& q1, const Quaternion& q2, float fT)
	{
		float fCos = q1.Dot(q2);
		if(fCos < 0.0f)
			*this = q1 + ((-q2) - q1)*fT;
		else
			*this = q1 + (q2 - q1)*fT;
		Normalize();
	}

	void Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float fT)//Perfect
	{
		float fCos = q1.Dot(q2);
		Quaternion rkT;
		if(fCos < 0.0f)
		{
			fCos = -fCos;
			rkT = -q2;
		}
		else
			rkT = q2;
		if(abs(fCos) < 1.0f - 1e-03)
		{
			float fSin = sqrt(1.0f - (fCos*fCos));
			float fAngle = atan2(fSin, fCos);
			float fInvSin = 1.0f / fSin;
			float fCoeff0 = sin((1.0f - fT) * fAngle) * fInvSin;
			float fCoeff1 = sin(fT * fAngle) * fInvSin;
			*this = q1*fCoeff0 + rkT*fCoeff1;
		}
		else
		{
			*this = q1 + (rkT - q1)*fT;
			Normalize();
		}
	}

	float Quaternion::GetPitch(bool reproject)
	{
		if(reproject)
		{
			float fTx = 2.0f*x;
			float fTy = 2.0f*y;
			float fTz = 2.0f*z;
			float fTwx = fTx*w;
			float fTxx = fTx*x;
			float fTyz = fTz*y;
			float fTzz = fTz*z;
			return atan2(fTyz + fTwx, 1.0f - (fTxx + fTzz));
		}
		return atan2(2.0f*(y*z + w*x), w*w - x*x - y*y + z*z);
	}

	float Quaternion::GetYaw(bool reproject)
	{
		if(reproject)
		{
			float fTx = 2.0f*x;
			float fTy = 2.0f*y;
			float fTz = 2.0f*z;
			float fTwy = fTy*w;
			float fTxx = fTx*x;
			float fTxz = fTz*x;
			float fTyy = fTy*y;
			return atan2(fTxz + fTwy, 1.0f - (fTxx + fTyy));
		}
		float val = -2.0f*(x*z - w*y);
		if(-1.0f < val)
		{
			if(val < 1.0f)
				return asin(val);
			return PI_HALF;
		}
		return -PI_HALF;
	}

	float Quaternion::GetRoll(bool reproject)
	{
		if(reproject)
		{
			float fTx = 2.0f*x;
			float fTy = 2.0f*y;
			float fTz = 2.0f*z;
			float fTwz = fTz*w;
			float fTxy = fTy*x;
			float fTyy = fTy*y;
			float fTzz = fTz*z;
			return atan2(fTxy + fTwz, 1.0f - (fTyy + fTzz));
		}
		return atan2(2.0f*(x*y + w*z), w*w + x*x - y*y - z*z);
	}
}