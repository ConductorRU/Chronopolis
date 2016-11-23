#pragma once
namespace DEN
{
	class Rect
	{
	public:
		int minX;
		int minY;
		int maxX;
		int maxY;
		Rect() { minX = minY = maxX = maxY = 0; }
		Rect(int minX, int minY, int maxX, int maxY)
		{
			this->minX = minX;
			this->minY = minY;
			this->maxX = maxX;
			this->maxY = maxY;
		}
	};
}