#pragma once
namespace DEN
{
	class Color;
	class ColorHSV;
	class ColorRGB;
	class ColorRGB
	{
	public:
		static const ColorRGB C_WHITE;
		static const ColorRGB C_YELLOW;
		static const ColorRGB C_RED;
		static const ColorRGB C_GREEN;
		static const ColorRGB C_BLUE;
		static const ColorRGB C_BLACK;
		static const ColorRGB C_GRAY;
		static const ColorRGB C_LILAC;
		union
		{
			uint color;
			struct
			{
				UCHAR r;
				UCHAR g;
				UCHAR b;
				UCHAR a;
			};
		};
		ColorRGB()
		{
			color = 0xffffffff;
		}
		ColorRGB(DWORD value)
		{
			color = value;
		}
		ColorRGB(const ColorHSV &col);
		ColorRGB(const Color &col);
		ColorRGB(int red, int green, int blue, int alpha = 255)
		{
			color = (uint)((((alpha) & 0xff) << 24) | (((blue) & 0xff) << 16) | (((green) & 0xff) << 8) | ((red) & 0xff));
		}
		inline void SetColor(int red, int green, int blue, int alpha = 255)
		{
			color = (uint)((((alpha) & 0xff) << 24) | (((blue) & 0xff) << 16) | (((green) & 0xff) << 8) | ((red) & 0xff));
		}
		inline ColorRGB operator + (const ColorRGB& p) const
		{
			int colR = (int)p.r + (int)r;
			int colG = (int)p.g + (int)g;
			int colB = (int)p.b + (int)b;
			int colA = (int)p.a + (int)a;
			colR = max(min(colR, 255), 0);
			colG = max(min(colG, 255), 0);
			colB = max(min(colB, 255), 0);
			colA = max(min(colA, 255), 0);
			return ColorRGB(colR, colG, colB, colA);
		}
		inline ColorRGB operator - (const ColorRGB& p) const
		{
			int colR = (int)r - (int)p.r;
			int colG = (int)g - (int)p.g;
			int colB = (int)b - (int)p.b;
			int colA = (int)a - (int)p.a;
			colR = max(min(colR, 255), 0);
			colG = max(min(colG, 255), 0);
			colB = max(min(colB, 255), 0);
			colA = max(min(colA, 255), 0);
			return ColorRGB(colR, colG, colB, colA);
		}
		static inline ColorRGB Lerp(ColorRGB start, ColorRGB end, float value)
		{
			if(value < 0.0f)
				value = 0.0f;
			else if(value > 1.0f)
				value = 1.0f;
			int colR = (int)start.r + (int)(((float)end.r - (float)start.r) / value);
			int colG = (int)start.g + (int)(((float)end.g - (float)start.g) / value);
			int colB = (int)start.b + (int)(((float)end.b - (float)start.b) / value);
			int colA = (int)start.a + (int)(((float)end.a - (float)start.a) / value);
			colR = max(min(colR, 255), 0);
			colG = max(min(colG, 255), 0);
			colB = max(min(colB, 255), 0);
			colA = max(min(colA, 255), 0);
			return ColorRGB(UCHAR(colR), UCHAR(colG), UCHAR(colB), UCHAR(colA));
		}
		inline bool operator == (const ColorRGB& p) const
		{
			return (color == p.color);
		}
		inline bool operator != (const ColorRGB& p) const
		{
			return (color != p.color);
		}
		inline void Add(ColorRGB col)
		{
			int colR = (int)col.r + (int)r;
			int colG = (int)col.g + (int)g;
			int colB = (int)col.b + (int)b;
			int colA = (int)col.a + (int)a;
			r = max(min(colR, 255), 0);
			g = max(min(colG, 255), 0);
			b = max(min(colB, 255), 0);
			a = max(min(colA, 255), 0);
		}
		inline bool operator < (const ColorRGB& col) const
		{
			return color < col.color;
		}
		inline bool operator > (const ColorRGB& col) const
		{
			return color > col.color;
		}
		Color ToColor() const;
		inline DWORD ToRGB() const
		{
			return RGB(r, g, b);
		}
	};
}