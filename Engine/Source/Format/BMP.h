#pragma once
namespace DEN
{
	class BMP
	{
	private:
		#pragma pack(push,1)
		typedef struct BMPHeader
		{
			WORD fType;        // 0x4d42 | 0x4349 | 0x5450
			int  fSize;        // размер файла
			WORD  fReserved1;    // 0
			WORD  fReserved2;    // 0
			int  fOffBits;     // смещение до поля данных, обычно 54 = 16 + biSize
			int  size;         // размер струкуры в байтах: 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER) или 124(BITMAPV5HEADER)
			uint  width;        // ширина в точках
			uint  height;       // высота в точках
			WORD planes;       // всегда должно быть 1
			WORD bitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
			int  compression;  // BI_RGB | BI_RLE8 | BI_RLE4 | BI_BITFIELDS | BI_JPEG | BI_PNG, реально используется лишь BI_RGB
			int  sizeImage;    // Количество байт в поле данных. Обычно устанавливается в 0
			int  xPelsPerMeter;// горизонтальное разрешение, точек на дюйм
			int  yPelsPerMeter;// вертикальное разрешение, точек на дюйм
			int  clrUsed;      // Количество используемых цветов (если есть таблица цветов)
			int  clrImportant; // Количество существенных цветов, Можно считать, просто 0
		} BMPHeader;
		#pragma pack(pop)
		uchar *data;
		uint width;
		uint height;
		uint size;
	public:
		BMP();
		~BMP();
		uint GetWidth() const { return width; }
		uint GetHeight() const { return height; }
		uint GetSize() const { return size; }
		uchar *GetData() const { return data; }
		static BMP *Load(const string &filename);
		static bool Save(const string &filename, uchar *data, uint sixeX, uint sizeY);
	};
}