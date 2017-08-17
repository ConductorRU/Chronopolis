#include "stdafx.h"
#include "../Material/Texture.h"
#include "Font.h"
namespace DEN
{
	Font::Font()
	{
		hFont = NULL;
		z_raw = NULL;
		z_width = 0;
		z_height = 0;
		_texture = nullptr;
	}
	Font::~Font()
	{
		if(hFont)
			DeleteObject(hFont);
		if(z_raw)
			delete[] z_raw;
		if(_texture)
			delete _texture;
	}
	void Font::InitChooseFont()
	{
		ZeroMemory(&lf, sizeof(lf));
		ZeroMemory(&cf, sizeof(cf));
		cf.lStructSize = sizeof(cf);
		cf.hwndOwner = GetActiveWindow();
		cf.lpLogFont = &lf;
		cf.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT;
	}
	void ErrMsgBox()
	{
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		MessageBoxA(NULL, (char*)lpMsgBuf, "GetLastError", MB_OK | MB_ICONINFORMATION);
		LocalFree(lpMsgBuf);
	}

	void Font::MeasureFont(HDC hdc)
	{
		GetTextMetricsA(hdc, &tm);// Measure TrueType fonts with GetCharABCWidths:
		if(!GetCharABCWidthsA(hdc, 0, 255, abc))// If it's not a TT font, use GetTextExtentPoint32 to fill array abc:
		{
			ZeroMemory(abc, 256 * sizeof(ABC));    // set abcA and abcC to 0 (abcB will be overwritten)
			SIZE Size;// for all characters, beginning with " ":
			for(int i = 0; i < 256; i++)
			{
				GetTextExtentPoint32A(hdc, (char*)&i, 1, &Size);// get width of character...
				abc[i].abcB = Size.cx;// ...and store it in abcB:
			}
		}
	}
	void Font::LoadFont(const string &family, int height, bool isBold, bool isItalic)
	{
		HWND hwnd = GetActiveWindow();
		InitChooseFont();
		if(hFont)// create an HFONT:
		{
			DeleteObject(hFont);
			hFont = NULL;
		}
		int wght = 400;
		if(isBold)
			wght = 700;
		hFont = CreateFontA(height, 0, 0, 0, wght, (DWORD)isItalic, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, family.c_str());
		//hFont = CreateFontIndirectA(&lf);
		HDC hdc = GetDC(hwnd);// get HDC:
		SelectObject(hdc, hFont);// select font:
		MeasureFont(hdc);// get text metrics and char widths:
		ReleaseDC(hwnd, hdc);// release HDC:
		RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);// redraw window:
		UINT x, y;
		void *v = GetRaw(x, y);
		_texture = new Texture;
		_texture->Create(x, y, RESOURCE_GPU);
		_texture->SetRaw((uchar*)v, x, y);
	}
	void Font::LoadFont()
	{
		HWND hwnd = GetActiveWindow();
		InitChooseFont();
		HDC hdc = GetDC(hwnd);
		GetObjectA(GetCurrentObject(hdc, OBJ_FONT), sizeof(LOGFONT), &lf);
		MeasureFont(hdc);
		ReleaseDC(hwnd, hdc);
		//if(ChooseFontA(&cf))
		{
			if(hFont)// create an HFONT:
			{
				DeleteObject(hFont);
				hFont = NULL;
			}
			hFont = CreateFontA(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, RUSSIAN_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, "Arial");
			//hFont = CreateFontIndirectA(&lf);
			HDC hdc = GetDC(hwnd);// get HDC:
			SelectObject(hdc, hFont);// select font:
			MeasureFont(hdc);// get text metrics and char widths:
			ReleaseDC(hwnd, hdc);// release HDC:
			RedrawWindow(hwnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE);// redraw window:
		}
	}
	const Rect &Font::GetRect(char c)
	{
		return z_rect[(UCHAR)c];
	}
	Square Font::GetSquare(char c)
	{
		Square s;
		if(z_width == 0 || z_height == 0)
			return s;
		s.minX = z_rect[(UCHAR)c].minX / (float)z_width;
		s.minY = z_rect[(UCHAR)c].minY / (float)z_height;
		s.maxX = z_rect[(UCHAR)c].maxX / (float)z_width;
		s.maxY = z_rect[(UCHAR)c].maxY / (float)z_height;
		return s;
	}
	//---------------------------------------------------------------------------

#define GetFilePointer(x) SetFilePointer(x, 0, 0, FILE_CURRENT)
#define ERROR_BREAK(x) { ErrMsgBox(); throw (int)(x); }
	char *Font::GetRaw(UINT &width, UINT &height, LPBITMAPINFO *lpb)
	{
		LPBITMAPINFO lpbi = NULL;
		HDC OffscrDC = NULL;
		z_raw = NULL;
		OffscrDC = CreateCompatibleDC(0);// Create an offscreen device context:
		if(hFont)
			SelectObject(OffscrDC, hFont);
		width = 16 * tm.tmMaxCharWidth;// Create an offscreen bitmap:
		height = 16 * tm.tmHeight;
		UINT pw = 2;
		while(pw < width)
			pw *= 2;
		width = pw;
		pw = 2;
		while(pw < height)
			pw *= 2;
		height = pw;
		z_width = width;
		z_height = height;
		HBITMAP OffscrBmp = CreateCompatibleBitmap(OffscrDC, width, height);
		if(!OffscrBmp)
			ERROR_BREAK(0);
		HBITMAP OldBmp = (HBITMAP)SelectObject(OffscrDC, OffscrBmp);// Select bitmap into DC:
		if(!OldBmp)
			ERROR_BREAK(1);
		SelectObject(OffscrDC, GetStockObject(BLACK_BRUSH));// Clear background to black:
		Rectangle(OffscrDC, 0, 0, width, height);
		SetBkMode(OffscrDC, TRANSPARENT);     // do not fill character background
		SetTextColor(OffscrDC, RGB(255, 255, 255)); // text color white
		unsigned char c;
		int posX, posY, maxX;
		for(int y = 0; y < 16; y++)
			for(int x = 0; x < 16; x++)
			{
				c = (unsigned char)(y * 16 + x);
				posX = x*tm.tmMaxCharWidth - abc[c].abcA;
				posY = y*tm.tmHeight;
				TextOutA(OffscrDC, posX, posY, (const char*)&c, 1);
				posX = x*tm.tmMaxCharWidth;
				maxX = posX + abc[c].abcB;
				z_rect[y * 16 + x].minX = posX;
				z_rect[y * 16 + x].minY = posY;
				z_rect[y * 16 + x].maxX = maxX;
				z_rect[y * 16 + x].maxY = y*tm.tmHeight + tm.tmHeight;
			}

		if((lpbi = (LPBITMAPINFO)(new char[sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD)])) == NULL)
			ERROR_BREAK(4);
		ZeroMemory(&lpbi->bmiHeader, sizeof(BITMAPINFOHEADER));
		lpbi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		lpbi->bmiHeader.biWidth = width;
		lpbi->bmiHeader.biHeight = height;
		lpbi->bmiHeader.biPlanes = 1;
		lpbi->bmiHeader.biBitCount = 32;
		lpbi->bmiHeader.biCompression = BI_RGB;
		lpbi->bmiHeader.biSizeImage = 0;
		lpbi->bmiHeader.biXPelsPerMeter = 0;
		lpbi->bmiHeader.biYPelsPerMeter = 0;
		lpbi->bmiHeader.biClrUsed = 0;
		lpbi->bmiHeader.biClrImportant = 0;
		// Get info but first de-select OffscrBmp because GetDIBits requires it:
		SelectObject(OffscrDC, OldBmp);
		if(!GetDIBits(OffscrDC, OffscrBmp, 0, height, NULL, lpbi, DIB_RGB_COLORS))
			ERROR_BREAK(5);

		// Reserve memory for bitmap bits:
		char *temp = new char[lpbi->bmiHeader.biSizeImage];
		if((z_raw = new char[lpbi->bmiHeader.biSizeImage]) == NULL)
			ERROR_BREAK(6);
		if(!GetDIBits(OffscrDC, OffscrBmp, 0, height, temp, lpbi, DIB_RGB_COLORS))
			ERROR_BREAK(7);
		for(UINT i = 0; i != height; ++i)
			memcpy(&z_raw[width * 4 * i], &temp[width*(height - (1 + i)) * 4], width * 4);
		for(UINT i = 0; i < width*height * 4; i += 4)
			z_raw[i + 3] = z_raw[i];
		delete[] temp;
		if(OffscrBmp) DeleteObject(OffscrBmp);
		if(lpb)
			*lpb = lpbi;
		return z_raw;
	}
	void Font::SaveFont()
	{
		HWND hwnd = GetActiveWindow();
		HDC OffscrDC = NULL;
		LPBITMAPINFO lpbi = NULL;
		LPVOID lpvBits = NULL;
		HANDLE BmpFile = INVALID_HANDLE_VALUE;
		OPENFILENAMEA OpenFileName;      // used for GetSaveFileName
		char SaveFileName[MAX_PATH];    // buffer for OpenFileName.lpstrFile
		try
		{
			ZeroMemory(&OpenFileName, sizeof(OpenFileName));
			OpenFileName.lStructSize = OPENFILENAME_SIZE_VERSION_400;   // = 76																										// (See remark at the end of this file!)
			OpenFileName.hwndOwner = hwnd;
			OpenFileName.lpstrFilter = "BMP Font files (*.bmp)\0*.bmp\0";
			OpenFileName.nFilterIndex = 1;
			OpenFileName.lpstrFile = SaveFileName;
			OpenFileName.nMaxFile = MAX_PATH;
			OpenFileName.lpstrTitle = "Save as BMP Font";
			OpenFileName.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;
			OpenFileName.lpstrDefExt = "bmp";
			strcpy_s(OpenFileName.lpstrFile, 32, lf.lfFaceName);///strcpy
			if(!GetSaveFileNameA(&OpenFileName))
				return;
			UINT width, height;
			lpvBits = GetRaw(width, height, &lpbi);

			// Create a file to save the DIB to:
			if((BmpFile = CreateFileA(OpenFileName.lpstrFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)) == INVALID_HANDLE_VALUE)
				ERROR_BREAK(8);
			DWORD Written;    // number of bytes written by WriteFile			
			BITMAPFILEHEADER bmfh;// Write a file header to the file:
			bmfh.bfType = 19778;        // 'BM'
																	// bmfh.bfSize = ???        // we'll write that later
																	// The bitmap format requires the Reserved fields to be 0.
																	// Our format is DDF. We store ID values into these fields:
			bmfh.bfReserved1 = 'DD';    // "DirectDraw...
			bmfh.bfReserved2 = 'FF';    // "...Font File"
																	// bmfh.bfOffBits = ???     // we'll write that later
			if(!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
				ERROR_BREAK(9);
			if(Written < sizeof(bmfh)) ERROR_BREAK(9);

			// Write BITMAPINFOHEADER to the file:
			if(!WriteFile(BmpFile, &lpbi->bmiHeader, sizeof(BITMAPINFOHEADER), &Written, NULL))
				ERROR_BREAK(10);
			if(Written < sizeof(BITMAPINFOHEADER))
				ERROR_BREAK(10);
			int PalEntries;// Calculate size of palette:
			if(lpbi->bmiHeader.biCompression == BI_BITFIELDS)// 16-bit or 32-bit bitmaps require bit masks:
				PalEntries = 3;
			else
				// bitmap is palettized?
				PalEntries = (lpbi->bmiHeader.biBitCount <= 8) ?
				// 2^biBitCount palette entries max.:
				1 << lpbi->bmiHeader.biBitCount
				// bitmap is TrueColor -> no palette:
				: 0;
			if(lpbi->bmiHeader.biClrUsed)// If biClrsUsed use only biClrUsed palette entries:
				PalEntries = lpbi->bmiHeader.biClrUsed;
			if(PalEntries)// Write palette to the file:
			{
				if(!WriteFile(BmpFile, &lpbi->bmiColors, PalEntries * sizeof(RGBQUAD), &Written, NULL))
					ERROR_BREAK(11);
				if(Written < PalEntries * sizeof(RGBQUAD))
					ERROR_BREAK(11);
			}

			// The current position in the file (at the beginning of the bitmap bits) will be saved to the BITMAPFILEHEADER:
			bmfh.bfOffBits = GetFilePointer(BmpFile);
			if(!WriteFile(BmpFile, lpvBits, lpbi->bmiHeader.biSizeImage, &Written, NULL))
				ERROR_BREAK(12);
			if(Written < lpbi->bmiHeader.biSizeImage)
				ERROR_BREAK(12);
			bmfh.bfSize = GetFilePointer(BmpFile);// The current pos. in the file is the final file size and will be saved:

																						// Store the font info and character widths to the file:
			if(WriteFile(BmpFile, &tm, sizeof(tm), &Written, NULL) &&
				WriteFile(BmpFile, abc, 256 * sizeof(ABC), &Written, NULL) &&
				WriteFile(BmpFile, &lf, sizeof(lf), &Written, NULL)
				== NULL) ERROR_BREAK(13);
			SetFilePointer(BmpFile, 0, 0, FILE_BEGIN);// We have all the info for the file header. Save the updated version:
			if(!WriteFile(BmpFile, &bmfh, sizeof(bmfh), &Written, NULL))
				ERROR_BREAK(14);
			if(Written < sizeof(bmfh))
				ERROR_BREAK(14);
		}
		catch(int &errorcode)
		{
			char Buf[100];
			wsprintfA(Buf, "Screenshot error #%i", errorcode);
			OutputDebugStringA(Buf);
		}
		catch(...)
		{
			OutputDebugStringA("Screenshot error");
		}
		if(OffscrDC) DeleteDC(OffscrDC);
		if(lpbi) delete[] lpbi;
		if(lpvBits) delete[] lpvBits;
		if(BmpFile != INVALID_HANDLE_VALUE) CloseHandle(BmpFile);
	};
};