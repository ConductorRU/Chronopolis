#include "stdafx.h"
#include "TGA.h"
namespace DEN
{
	TGA::TGA()
	{
		data = nullptr;
		width = height = size = 0;
	}
	TGA::~TGA()
	{
		delete[] data;
	}

	TGA *TGA::Load(const string &filename)
	{
		return nullptr;
	}
	bool TGA::Save(const string &filename, uchar *data, uint sixeX, uint sizeY)
	{
		return false;
	}
};