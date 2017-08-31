#pragma once
namespace DEN
{
	class Color
	{
	public:
		float r, g, b, a;
		Color() {r = g = b = a = 1.0f;};
		Color::Color(float red, float green, float blue, float alpha = 1.0f)
		{
			r = red;
			g = green;
			b = blue;
			a = alpha;
		}
		Color::Color(const string &val)
		{
			FromHex(val);
		}
		Color::Color(int red, int green, int blue, int alpha = 255)
		{
			r = (float)red / 255.0f;
			g = (float)green / 255.0f;
			b = (float)blue / 255.0f;
			a = (float)alpha / 255.0f;
		}
		static const Color C_WHITE;
		static const Color C_YELLOW;
		static const Color C_RED;
		static const Color C_GREEN;
		static const Color C_BLUE;
		static const Color C_BLACK;
		static const Color C_GRAY;
		static const Color C_LILAC;
		const bool operator== (const Color &col)
		{
			if(r == col.r && g == col.g && b == col.b && a == col.a)
				return true;
			return false;
		}
		const bool operator!= (const Color &col)
		{
			if(r != col.r || g != col.g || b != col.b || a != col.a)
				return true;
			return false;
		}
		static inline uint ToD3DColor(int red, int green, int blue, int alpha = 255)
		{
			return (uint)((((alpha) & 0xff) << 24) | (((blue) & 0xff) << 16) | (((green) & 0xff) << 8) | ((red) & 0xff));
		}
		inline uint ToD3DColor()
		{
			int colR = (int)(r*255.0f);
			int colG = (int)(g*255.0f);
			int colB = (int)(b*255.0f);
			int colA = (int)(a*255.0f);
			colR = max(min(colR, 255), 0);
			colG = max(min(colG, 255), 0);
			colB = max(min(colB, 255), 0);
			colA = max(min(colA, 255), 0);
			return (uint)((((colA) & 0xff) << 24) | (((colB) & 0xff) << 16) | (((colG) & 0xff) << 8) | ((colR) & 0xff));
		}
		class ColorRGB ToRGBA();
		ColorRGB ToRGB();
		inline void Add(const Color &color)
		{
			r += color.r;
			g += color.g;
			b += color.b;
			a += color.a;
		}
		inline void Multiple(const Color &color)
		{
			r *= color.r;
			g *= color.g;
			b *= color.b;
			a *= color.a;
		}
		inline void AlphaMask(const Color &color)
		{
			a = (color.r + color.g + color.b) / 3.0f;
		}
		inline void AlphaAdd(const Color &color)
		{
			r = r*(1.0f - color.a);
			g = g*(1.0f - color.a);
			b = b*(1.0f - color.a);
			r += color.r*color.a;
			g += color.g*color.a;
			b += color.b*color.a;
		}
		void Lerp(const Color &op1, const Color &op2, float f);
		bool FromHex(const string &hex);
	};
}