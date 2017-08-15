#pragma once
#include "Format.h"
namespace DEN
{
	class TGA: public ImageFormat
	{
	public:
		static TGA *Load(const string &filename);
		static bool Save(const string &filename, uchar *data, uint sixeX, uint sizeY);
	};
}