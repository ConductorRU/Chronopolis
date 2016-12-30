#pragma once
namespace DEN
{
	class Point2
	{
	public:
		int x, y;
		Point2()
		{
			x = y = 0;
		}
		inline Point2(int pX, int pY)
		{
			x = pX;
			y = pY;
		}
		~Point2()
		{

		}
		inline Point2 operator + (const  Point2 &pVector)
		{
			return Point2(x + pVector.x, y + pVector.y);
		}
		inline Point2 operator - (const  Point2 &pVector)
		{
			return Point2(x - pVector.x, y - pVector.y);
		}
		inline Point2 operator * (const Point2 &pVector)
		{
			return Point2(x*pVector.x, y*pVector.y);
		}
		inline Point2 operator / (const Point2 &pVector)
		{
			return Point2(x / pVector.x, y / pVector.y);
		}
		inline Point2 operator / (float f)
		{
			return Point2((int)((float)x / f), (int)((float)y / f));
		}
		inline Point2 operator * (float f)
		{
			return Point2((int)((float)x*f), (int)((float)y*f));
		}
		inline Point2 operator / (int i)
		{
			return Point2(x / i, y / i);
		}
		inline Point2 operator * (int i)
		{
			return Point2(x*i, y*i);
		}
		inline bool operator == (const Point2& point) const
		{
			return (x == point.x) && (y == point.y);
		}
		inline bool operator!= (const Point2 &s) const
		{
			if(x != s.x || y != s.y)
				return true;
			return false;
		}
		inline bool operator < (const Point2& point) const
		{
			if(x < point.x) return true;
			if(x == point.x && y < point.y) return true;
			return false;
		}
		inline bool operator > (const Point2& point) const
		{
			if(x > point.x) return true;
			if(x == point.x && y > point.y) return true;
			return false;
		}
		inline float GetDistance(const Point2 &destVector) const
		{
			return sqrt((float)((x - destVector.x)*(x - destVector.x) + (y - destVector.y)*(y - destVector.y)));
		}
		void Lerp(const Point2& p1, const Point2& p2, const float f)
		{
			if(f < 0.0f)
				*this = p1;
			else if(f > 1.0f)
				*this = p2;
			x = p1.x + int(f*float(p2.x - p1.x) + 0.5f);
			y = p1.y + int(f*float(p2.y - p1.y) + 0.5f);
		}
	};
}