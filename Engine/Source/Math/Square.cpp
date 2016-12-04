#include "stdafx.h"
#include "Square.h"
namespace DEN
{
	bool Square::Pick(const Point2 &p) const
	{
		if(minX <= p.x && maxX > p.x && minY <= p.y && maxY > p.y)
			return true;
		return false;
	}
	bool Square::PickSize(const Point2 &p) const
	{
		if(minX <= p.x && (minX + maxX) > p.x && minY <= p.y && (minY + maxY) > p.y)
			return true;
		return false;
	}
}