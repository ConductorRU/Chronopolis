#include "stdafx.h"
#include "Func.h"
#include "ColorHSV.h"
#include "Color.h"
#include "ColorRGB.h"
namespace DEN
{
	const ColorRGB ColorRGB::C_WHITE = ColorRGB(255, 255, 255, 255);
	const ColorRGB ColorRGB::C_YELLOW = ColorRGB(255, 255, 0, 255);
	const ColorRGB ColorRGB::C_RED = ColorRGB(255, 0, 0, 255);
	const ColorRGB ColorRGB::C_GREEN = ColorRGB(0, 255, 0, 255);
	const ColorRGB ColorRGB::C_BLUE = ColorRGB(0, 0, 255, 255);
	const ColorRGB ColorRGB::C_BLACK = ColorRGB(0, 0, 0, 255);
	const ColorRGB ColorRGB::C_GRAY = ColorRGB(127, 127, 127, 255);
	const ColorRGB ColorRGB::C_LILAC = ColorRGB(200, 127, 255, 255);
	ColorRGB::ColorRGB(const ColorHSV &col)
	{
		//float c = (1.0f - abs(2.0f*col.l - 1.0f))*col.s;//0..1
		float c = col.v*col.s;
		float fH = col.h / 60.0f;//0..6
		float fh = fH - float((int(fH) / 2) * 2);
		float x = c*(1.0f - abs(fh - 1.0f));//
		float R = 0, G = 0, B = 0;
		if(0.0f <= fH && fH < 1.0f)
			R = c, G = x;
		else if(1.0f <= fH && fH < 2.0f)
			R = x, G = c;
		else if(2.0f <= fH && fH < 3.0f)
			G = c, B = x;
		else if(3.0f <= fH && fH < 4.0f)
			G = x, B = c;
		else if(4.0f <= fH && fH < 5.0f)
			R = x, B = c;
		else if(5.0f <= fH && fH < 6.0f)
			R = c, B = x;
		float m = col.v - c;
		R += m;
		G += m;
		B += m;
		r = Clamp(toInt(R*255.0f), 0, 255);
		g = Clamp(toInt(G*255.0f), 0, 255);
		b = Clamp(toInt(B*255.0f), 0, 255);
		a = Clamp(toInt(col.a*255.0f), 0, 255);
	}

	ColorRGB::ColorRGB(const Color &col)
	{
		int colR = toInt(col.r*255.0f);
		int colG = toInt(col.g*255.0f);
		int colB = toInt(col.b*255.0f);
		int colA = toInt(col.a*255.0f);
		r = UCHAR(max(min(colR, 255), 0));
		g = UCHAR(max(min(colG, 255), 0));
		b = UCHAR(max(min(colB, 255), 0));
		a = UCHAR(max(min(colA, 255), 0));
	}

	Color ColorRGB::ToColor() const
	{
		return Color(r, g, b, a);
	}
}