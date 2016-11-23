#pragma once
namespace DEN
{
	class Vector;
	class Matrix;
	class Matrix2D;
	class Quaternion
	{
	public:
		float x, y, z, w;
		inline Quaternion(float fx = 0.0f, float fy = 0.0f, float fz = 0.0f, float fw = 1.0f)
		{
			x = fx;
			y = fy;
			z = fz;
			w = fw;
		}
		Quaternion(float rad, const Vector& axis);
		Quaternion(float radX, float radY, const Vector& axis);
		inline Quaternion& operator= (const Quaternion& quat)
		{
			w = quat.w;
			x = quat.x;
			y = quat.y;
			z = quat.z;
			return *this;
		}
		Quaternion operator+ (const Quaternion& quat) const;
		Quaternion operator- (const Quaternion& quat) const;
		Quaternion operator* (const Quaternion& quat) const;
		Quaternion operator* (float f) const;
		Vector operator* (const Vector& p) const;
		Quaternion operator- () const;
		inline bool operator== (const Quaternion& quat) const
		{
			return (quat.x == x) && (quat.y == y) && (quat.z == z) && (quat.w == w);
		}
		inline bool operator!= (const Quaternion& quat) const
		{
			return !operator==(quat);
		}
		Quaternion Quaternion::Inverse() const;
		Vector ToVector();
		void ToAngleAxis(float& angle, Vector& axis) const;
		void Quaternion::FromAngleAxis(const float& rfAngle, const Vector& rkAxis);
		void ToAxes(Vector& xAxis, Vector& yAxis, Vector& zAxis) const;
		void ToRotationMatrix(class Matrix& kRot) const;
		void ToRotationMatrix(class Matrix2D& kRot) const;
		float Normalize();
		void FromRotationMatrix(const Matrix& kRot);
		void FromRotationMatrix(const Matrix2D& kRot);
		void FromAxes(const Vector& xaxis, const Vector& yaxis, const Vector& zaxis);
		float Dot(const Quaternion& q) const;
		void Nlerp(const Quaternion& q1, const Quaternion& q2, float fT);
		void Quaternion::Slerp(const Quaternion& q1, const Quaternion& q2, float fT);
		static const Quaternion IDENTITY;
		static const Quaternion ZERO;
		float GetPitch(bool reproject = true);
		float GetYaw(bool reproject = true);
		float GetRoll(bool reproject = true);
	};
}