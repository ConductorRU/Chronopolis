#include "stdafx.h"
#include "BMP.h"
namespace DEN
{
	BMP *BMP::Load(const string &filename)
	{
		FILE *f = nullptr;
		fopen_s(&f, filename.c_str(), "rb");
		if (!f)
			return nullptr;
		BMPHeader bh; // File header sizeof(BMPheader) = 56
		size_t res;

		res = fread(&bh, 1, sizeof(BMPHeader), f);// читаем заголовок
		if (res != sizeof(BMPHeader))
		{
			fclose(f);
			return nullptr;
		}
		if(bh.fType != 0x4d42 && bh.fType != 0x4349 && bh.fType != 0x5450)// проверяем сигнатуру
		{
			fclose(f);
			return nullptr;
		}
		
		fseek(f, 0, SEEK_END);// проверка размера файла
		int filesize = ftell(f);
		fseek(f, sizeof(BMPHeader), SEEK_SET);// проверим условия
		if(bh.fSize != filesize || bh.fReserved1 != 0|| bh.fReserved2 != 0 || bh.planes != 1 || (bh.size != 40 && bh.size != 108 && bh.size != 124) ||
			bh.fOffBits != 14 + bh.size || bh.width <1 || bh.width >10000 || bh.height<1 || bh.height>10000
			|| bh.bitCount != 24 || bh.compression != 0)// пока рассматриваем только полноцветные, несжатые изображения
		{
			fclose(f);
			return nullptr;
		}
		
		uint mx = bh.width; // Заголовок прочитан и проверен, тип - верный (BGR-24), размеры (mx,my) найдены
		uint my = bh.height;
		uint mx3 = (3 * mx + 3) & (-4); // Compute row width in file, including padding to 4-byte boundary
		unsigned char *tmp_buf = new unsigned char[mx3*my]; // читаем данные
		res = fread(tmp_buf, 1, mx3*my, f);
		if ((int)res != mx3*my)
		{
			delete[]tmp_buf;
			fclose(f);
			return nullptr;
		}
		fclose(f);// данные прочитаны
		uchar *v = new uchar[mx*my*4];// выделим память для результата
		uchar *ptr = v;// Перенос данных (не забудем про BGR->RGB)
		for (int y = my - 1; y >= 0; y--)
		{
			unsigned char *pRow = tmp_buf + mx3*y;
			for (uint x = 0; x < mx; ++x)
			{
				*ptr++ = *(pRow + 2);
				*ptr++ = *(pRow + 1);
				*ptr++ = *pRow;
				pRow += 3;
				*ptr++ = 255;
			}
		}
		delete[] tmp_buf;
		BMP *bmp = new BMP;
		bmp->width = bh.width;
		bmp->height = bh.height;
		bmp->size = mx*my*4;
		bmp->data = v;
		return bmp;    // OK
	}
	bool BMP::Save(const string &filename, uchar *data, uint sizeX, uint sizeY)
	{
		BMPHeader bh;
		memset(&bh, 0, sizeof(bh));
		bh.fType =0x4d42;	// 'BM'
		int mx3 = (3*sizeX+3) & (-4);// Найдем длину строки в файле, включая округление вверх до кратного 4:
		int filesize = 54 + sizeY*mx3;
		bh.fSize = filesize;
		bh.fReserved1 =  0;
		bh.fReserved2 =  0;
		bh.planes   =  1;
		bh.size     = 40;
		bh.fOffBits  = 14 + bh.size;
		bh.width    = sizeX;
		bh.height   = sizeY;
		bh.bitCount = 24;
		bh.compression = 0;

		FILE *f = nullptr;
		fopen_s(&f, filename.c_str(), "wb");
		if(!f)
			return 0;
		size_t res;
		res = fwrite(&bh, 1, sizeof(BMPHeader), f);
		if(res != sizeof(BMPHeader))
		{
			fclose(f);
			return false;
		}

		unsigned char *tmp_buf = new unsigned char[mx3*sizeY];// приготовим временный буфер
		uchar *ptr = data;// Перенос данных (не забудем про RGB->BGR)
		for(int y = sizeY - 1; y >= 0; --y)
		{
			unsigned char *pRow = tmp_buf + mx3*y;
			for(uint x = 0; x < sizeX; ++x)
			{
				*(pRow + 2) = *ptr++;
				*(pRow + 1) = *ptr++;
				*pRow       = *ptr++; 
				pRow+=3;
				ptr++;
			}
		}
		fwrite(tmp_buf, 1, mx3*sizeY, f);
		fclose(f);
		delete[] tmp_buf;
		return true;
	}
};