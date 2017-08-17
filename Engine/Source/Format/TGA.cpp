#include "stdafx.h"
#include "TGA.h"
namespace DEN
{

	TGA *TGA::Load(const string &filename)
	{
		uchar head[12], tga[6];
		uchar uTGAcompare[12] = {0,0, 2,0,0,0,0,0,0,0,0,0};
		uchar cTGAcompare[12] = {0,0,10,0,0,0,0,0,0,0,0,0};
		FILE * fTGA = nullptr; // ��������� ��������� �� ����
		fopen_s(&fTGA, filename.c_str(), "rb"); // ��������� ���� �� ������
		if(!fTGA)
			return nullptr;
		if(fread(&head, sizeof(head), 1, fTGA) == 0)
			return nullptr;
		if(fread(tga, sizeof(tga), 1, fTGA) == 0)
			return nullptr;
		TGA *file = new TGA;
		file->width  = tga[1] * 256 + tga[0];  // ��������� ������
		file->height = tga[3] * 256 + tga[2];  // ��������� ������
		uint bpp = tga[4];       // ��������� ���������� ��� �� ������� � ��������� ���������
		uint bytepp = bpp/8;
		file->size = file->width*file->height*bytepp;
		if((file->width <= 0) || (file->height <= 0) || ((bpp != 24) && (bpp !=32)))
		{
			delete file;
			return nullptr;
		}
		uchar *data = new uchar[file->size];
		if(memcmp(uTGAcompare, &head, sizeof(head)) == 0)//��������
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
		else if(memcmp(cTGAcompare, &head, sizeof(head)) == 0)//������
		{
			uint pixelcount = file->width*file->height; // ���������� �������� � �����������
			uint curPixel = 0;
			uint curByte = 0;
			uchar *colorbuffer = new uchar[bytepp];// ��������� ��� ������ �������
			do
			{
				uchar chunkheader = 0;
				if(fread(&chunkheader, sizeof(uchar), 1, fTGA) == 0)
				{
					delete file;
					return nullptr;
				}
				if(chunkheader < 128)   // ���� ������ �������� 'RAW' �������
				{                         
					chunkheader++;        // ��������� ������� ��� ��������� ���������� RAW ��������
					for(short counter = 0; counter < chunkheader; counter++)
					{
						if(fread(colorbuffer, 1, bytepp, fTGA) != bytepp)// �������� ��������� 1 �������
						{
							delete file;
							return nullptr;
						}
						data[curByte] = colorbuffer[0];       // �������� ���� 'R'
						data[curByte + 1] = colorbuffer[1]; // �������� ���� 'G'
						data[curByte + 2] = colorbuffer[2]; // �������� ���� 'B'
						if(bytepp == 4)          // ���� ��� 32bpp �����������...
							data[curByte + 3] = colorbuffer[3];  // �������� ���� 'A'
						// ����������� ������� ������ �� �������� ������ ���������� ���� �� �������
						curByte += bytepp;
						++curPixel; // ����������� ���������� �������� �� 1
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
						data[curByte] = colorbuffer[0];// �������� ���� 'R'
						data[curByte + 1] = colorbuffer[1];// �������� ���� 'G'
						data[curByte + 2] = colorbuffer[2];// �������� ���� 'B'
						if(bytepp == 4)    // ���� ��� 32bpp �����������
							data[curByte + 3] = colorbuffer[3];
						curByte += bytepp;  // �������������� ������� ������
						curPixel++;        // �������������� ������� ��������
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