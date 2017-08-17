#include "stdafx.h"
#include "Format.h"
namespace DEN
{
	ImageFormat::ImageFormat()
	{
		data = nullptr;
		width = height = size = 0;
	}
	ImageFormat::~ImageFormat()
	{
		delete[] data;
	}
}