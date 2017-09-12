#pragma once
#include "Point2.h"
namespace DEN
{
	struct Square
	{
		union { float minX; float left; float left_top;};
		union { float maxX; float right; float right_bottom;};
		union { float minY; float top; float right_top;};
		union { float maxY; float bottom; float left_bottom;};
		const bool operator== (const Square &s)
		{
			if(minX == s.minX && minY == s.minY && maxX == s.maxX && maxY == s.maxY)
				return true;
			return false;
		}
		const bool operator!= (const Square &s)
		{
			if(minX != s.minX || minY != s.minY || maxX != s.maxX || maxY != s.maxY)
				return true;
			return false;
		}
		Square Square::operator+ (const Square& s) const
		{
			return Square(minX + s.minX, minY + s.minY, maxX + s.maxX, maxY + s.maxY);
		}
		Square() { minX = minY = maxX = maxY = 0.0f; }
		Square(float minX, float minY, float maxX, float maxY)
		{
			this->minX = minX;
			this->minY = minY;
			this->maxX = maxX;
			this->maxY = maxY;
		}
		bool Pick(const Point2 &p) const;
		bool PickSize(const Point2 &p) const;
	};
}