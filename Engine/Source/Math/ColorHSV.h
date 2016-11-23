#pragma once
namespace DEN
{
	class ColorRGB;
	class ColorHSV
	{
	public:
		float h;
		float s;
		float v;
		float a;
		ColorHSV()
		{
			h = 0.0f;
			s = 1.0f;
			v = 1.0f;
			a = 1.0f;
		}
		ColorHSV(float hue, float saturation, float value, float alpha)
		{
			h = hue;
			s = saturation;
			v = value;
			a = alpha;
		}
		ColorHSV(const ColorRGB &col);
	};
}