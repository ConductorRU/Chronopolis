#pragma once
#include "../Math/Rect.h"
#include "../Math/Square.h"
namespace DEN
{
	class Texture;
	class Font
	{
	private:
		void InitChooseFont();
		void MeasureFont(HDC hdc);
		char *z_raw;
		Rect z_rect[256];
		UINT z_width;
		UINT z_height;
		Texture *_texture;
	public:
		HFONT hFont;                // HFONT used to draw with this font
		LOGFONTA lf;                 // font description for CreateFontIndirect
		CHOOSEFONTA cf;              // font description for ChooseFont dialog
		TEXTMETRICA tm;              // text metrics, e.g. character height
		ABC abc[256];               // character widths
		Texture *GetTexture() {return _texture;}
		Font();
		~Font();
		char *GetRaw(UINT &width, UINT &height, LPBITMAPINFO *lpbi = NULL);
		void SaveFont();
		void LoadFont(const string &family, int height, bool isBold = false, bool isItalic = false);
		void LoadFont();
		const Rect &GetRect(char c);
		Square GetSquare(char c);
	};
};