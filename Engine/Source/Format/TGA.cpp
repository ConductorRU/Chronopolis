#include "stdafx.h"
#include "TGA.h"
namespace DEN
{

	TGA *TGA::Load(const string &filename)
	{
		uchar head[12], tga[6];
		uchar uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
		uchar cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};
		FILE * fTGA = nullptr; // Объявляем указатель на файл
		fopen_s(&fTGA, filename.c_str(), "rb"); // Открываем файл на чтение
		if(!fTGA)
			return nullptr;
		if(fread(&head, sizeof(head), 1, fTGA) == 0)
			return nullptr;
		if(fread(tga, sizeof(tga), 1, fTGA) == 0)
			return nullptr;
		TGA *file = new TGA;
		file->width  = tga[1] * 256 + tga[0];  // Вычисляем высоту
		file->height = tga[3] * 256 + tga[2];  // Вычисляем ширину
		uint bpp = tga[4];       // Вычисляем количество бит на пиксель в локальной структуре
		uint bytepp = bpp/8;
		file->size = file->width*file->height*bytepp;
		if((file->width <= 0) || (file->height <= 0) || ((bpp != 24) && (bpp !=32)))
		{
			delete file;
			return nullptr;
		}
		uchar *data = new uchar[file->size];
		if(memcmp(uTGAcompare, &head, sizeof(head)) == 0)//несжатый
		{
			if(fread(data, 1, file->size, fTGA) != file->size)
			{
				delete file;
				return nullptr;
			}
			file->data = new uchar[file->width*file->height*4];
			if(bytepp == 4)
				memcpy(file->data, data, file->size);
			else for (uint y = 0; y < file->height; ++y)
			{
				uchar *ptr = file->data;
				unsigned char *pRow = data + file->width*y*bytepp;
				for(uint x = 0; x < file->width; ++x)
				{
					*ptr++ = *(pRow);
					*ptr++ = *(pRow + 1);
					*ptr++ = *(pRow + 2);
					*ptr++ = 255;
					pRow += 3;
				}
			}
			fclose(fTGA);
			return file;
		}
		else if(memcmp(cTGAcompare, &head, sizeof(head)) == 0)//сжатый
		{
			uint pixelcount = file->width*file->height; // Количество пикселей в изображении
			uint curPixel = 0;
			uint curByte = 0;
			uchar *colorbuffer = new uchar[bytepp];// Хранилище для одного пикселя
			do
			{
				uchar chunkheader = 0;
				if(fread(&chunkheader, sizeof(uchar), 1, fTGA) == 0)
				{
					delete file;
					return nullptr;
				}
				if(chunkheader < 128)   // Если секция является 'RAW' секцией
				{                         
					chunkheader++;        // Добавляем единицу для получения количества RAW пикселей
					for(short counter = 0; counter < chunkheader; counter++)
					{
						if(fread(colorbuffer, 1, bytepp, fTGA) != bytepp)// Пытаемся прочитать 1 пиксель
						{
							delete file;
							return nullptr;
						}
						data[curByte] = colorbuffer[0];       // Записать байт 'R'
						data[curByte + 1] = colorbuffer[1]; // Записать байт 'G'
						data[curByte + 2] = colorbuffer[2]; // Записать байт 'B'
						if(bytepp == 4)          // Если это 32bpp изображение...
							data[curByte + 3] = colorbuffer[3];  // Записать байт 'A'
						// Увеличиваем счетчик байтов на значение равное количеству байт на пиксель
						curByte += bytepp;
						++curPixel; // Увеличиваем количество пикселей на 1
					}
				}
				else
				{
					chunkheader -= 127;
					if(fread(colorbuffer, 1, bytepp, fTGA) != bytepp)
					{ 
						delete file;
						return nullptr;
					}
					for(short counter = 0; counter < chunkheader; counter++)
					{
						data[curByte] = colorbuffer[0];// Копируем байт 'R'
						data[curByte + 1] = colorbuffer[1];// Копируем байт 'G'
						data[curByte + 2] = colorbuffer[2];// Копируем байт 'B'
						if(bytepp == 4)    // Если это 32bpp изображение
							data[curByte + 3] = colorbuffer[3];
						curByte += bytepp;  // Инкрементируем счетчик байтов
						curPixel++;        // Инкрементируем счетчик пикселей
					}
				}
			}
			while(curPixel < pixelcount);
			file->data = data;
			fclose(fTGA);
			return file;
		}
		return nullptr;
	}
	bool TGA::Save(const string &filename, uchar *data, uint sixeX, uint sizeY)
	{
		return false;
	}
};