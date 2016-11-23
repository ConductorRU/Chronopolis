#include "stdafx.h"
#include "Func.h"
#include "ColorRGB.h"
#include "Color.h"
namespace DEN
{
	class ColorRGB;
	ColorRGB Color::ToRGBA()
	{
		ColorRGB col;
		int colR = (int)(r*255.0f);
		int colG = (int)(g*255.0f);
		int colB = (int)(b*255.0f);
		int colA = (int)(a*255.0f);
		col.r = max(min(colR, 255), 0);
		col.g = max(min(colG, 255), 0);
		col.b = max(min(colB, 255), 0);
		col.a = max(min(colA, 255), 0);
		return col;
	}

	ColorRGB Color::ToRGB()
	{
		ColorRGB col;
		int colR = (int)(r*255.0f);
		int colG = (int)(g*255.0f);
		int colB = (int)(b*255.0f);
		col.r = max(min(colR, 255), 0);
		col.g = max(min(colG, 255), 0);
		col.b = max(min(colB, 255), 0);
		col.a = 255;
		return col;
	}
	void Color::Lerp(const Color &op1, const Color &op2, float f)
	{
		r = lerp(op1.r, op2.r, f);
		g = lerp(op1.g, op2.g, f);
		b = lerp(op1.b, op2.b, f);
		a = lerp(op1.a, op2.a, f);
	}
}