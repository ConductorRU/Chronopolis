#include "stdafx.h"
#include "Vector.h"
#include "Matrix.h"
#include "BBox.h"
namespace DEN
{
	BBox::BBox()
	{
	}

	BBox::BBox(Vector min, Vector max)
	{
		z_min = min;
		z_max = max;
		z_oMin = z_min;
		z_oMax = z_max;
	}

	BBox::~BBox()
	{
	}

	Vector BBox::GetMin() const
	{
		return z_oMin;
	}

	Vector BBox::GetMax() const
	{
		return z_oMax;
	}

	Vector BBox::GetOrigMin() const
	{
		return z_min;
	}

	Vector BBox::GetOrigMax() const
	{
		return z_max;
	}

	float BBox::GetMinSize() const
	{
		return min((z_oMax.x - z_oMin.x), min((z_oMax.y - z_oMin.y), (z_oMax.z - z_oMin.z)));
	}

	float BBox::GetMaxSize() const
	{
		return max((z_oMax.x - z_oMin.x), max((z_oMax.y - z_oMin.y), (z_oMax.z - z_oMin.z)));
	}

	float BBox::GetRadius() const
	{
		return GetCenter().GetDistance(z_oMax);
	}

	float BBox::GetLength() const
	{
		return z_oMax.GetDistance(z_oMin);
	}

	Vector BBox::GetNormalizeLength() const
	{
		Vector v = z_oMax - z_oMin;
		Vector n = v;
		n.Normalize();
		return n;
	}

	Vector BBox::GetCenter() const
	{
		return (z_oMax + z_oMin)*0.5f;
	}

	Vector BBox::GetMid() const
	{
		return (z_oMax - z_oMin)*0.5f;
	}

	Vector BBox::GetSize() const
	{
		return (z_oMax - z_oMin);
	}

	void BBox::SetBox(const Vector &min, const Vector &max)
	{
		z_min = min;
		z_max = max;
		z_oMin = z_min;
		z_oMax = z_max;
	}

	void BBox::UpdateMatrix(Matrix *matrix)
	{
		z_oMin = matrix->TransformCoord(z_min);
		z_oMax = matrix->TransformCoord(z_max);
		Vector m = z_oMin;
		if(z_oMax.x < m.x)
		{
			z_oMin.x = z_oMax.x;
			z_oMax.x = m.x;
		}
		if(z_oMax.y < m.y)
		{
			z_oMin.y = z_oMax.y;
			z_oMax.y = m.y;
		}
		if(z_oMax.z < m.z)
		{
			z_oMin.z = z_oMax.z;
			z_oMax.z = m.z;
		}
	}

	bool BBox::Intersect(const Vector &point)
	{
		if(z_oMin.x < point.x && z_oMax.x > point.x)
			if(z_oMin.y < point.y && z_oMax.y > point.y)
				if(z_oMin.z < point.z && z_oMax.z > point.z)
					return true;
		return false;
	}

	bool BBox::IntersectSphere(const Vector &point)
	{
		if(GetCenter().GetDistance(point) <= (z_oMax - z_oMin).Length())
			return true;
		return false;
	}
}