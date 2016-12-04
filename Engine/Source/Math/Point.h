#pragma once
namespace DEN
{
	struct Point
	{
		int x;
		int y;
		int z;
		Point() { x = y = z = 0; }
		Point(int x, int y, int z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}
		const bool operator== (const Point &s)
		{
			if(x == s.x && y == s.y && z == s.z)
				return true;
			return false;
		}
		const bool operator!= (const Point &s)
		{
			if(x != s.x || y != s.y || z != s.z)
				return true;
			return false;
		}
	};
}