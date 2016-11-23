#include "stdafx.h"
#include "ColorRGB.h"
#include "ColorHSV.h"
namespace DEN
{
	ColorHSV::ColorHSV(const ColorRGB &col)
	{
		float r = float(col.r) / 255.0f;
		float g = float(col.g) / 255.0f;
		float b = float(col.b) / 255.0f;
		float cMax = max(max(r, g), b);
		float cMin = min(min(r, g), b);
		float del = cMax - cMin;
		h = 0.0f;
		if(del > 0.000001f)
		{
			if(r >= g && r >= b)
				h = 60.0f*((g - b) / del);
			else if(g >= r && g >= b)
				h = 60.0f*((b - r) / del + 2.0f);
			else
				h = 60.0f*((r - g) / del + 4.0f);
			h = h - float((int(h) / 360) * 360);
		}
		v = cMax;
		s = del;
		if(del > 0.0f)
			s /= v;
		a = float(col.a) / 255.0f;
	}
}