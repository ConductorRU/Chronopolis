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
			int  fSize;        // ������ �����
			WORD  fReserved1;    // 0
			WORD  fReserved2;    // 0
			int  fOffBits;     // �������� �� ���� ������, ������ 54 = 16 + biSize
			int  size;         // ������ �������� � ������: 40(BITMAPINFOHEADER) ��� 108(BITMAPV4HEADER) ��� 124(BITMAPV5HEADER)
			uint  width;        // ������ � ������
			uint  height;       // ������ � ������
			WORD planes;       // ������ ������ ���� 1
			WORD bitCount;     // 0 | 1 | 4 | 8 | 16 | 24 | 32
			int  compression;  // BI_RGB | BI_RLE8 | BI_RLE4 | BI_BITFIELDS | BI_JPEG | BI_PNG, ������� ������������ ���� BI_RGB
			int  sizeImage;    // ���������� ���� � ���� ������. ������ ��������������� � 0
			int  xPelsPerMeter;// �������������� ����������, ����� �� ����
			int  yPelsPerMeter;// ������������ ����������, ����� �� ����
			int  clrUsed;      // ���������� ������������ ������ (���� ���� ������� ������)
			int  clrImportant; // ���������� ������������ ������, ����� �������, ������ 0
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