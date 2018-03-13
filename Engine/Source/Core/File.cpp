#include "stdafx.h"
#include "File.h"
#include "../Math/Vector.h"
#include "../Math/Color.h"
#include "../Math/ColorRGB.h"
#include "../Math/Matrix.h"
#include "../Math/Quaternion.h"
#include "../Math/Rect.h"
#include "../Math/Point.h"
namespace DEN
{
	File::File()
	{

	}

	File::~File()
	{
		if(_file.is_open())
			_file.close();
	}

	bool File::Load(const string &fileName, bool binary, char **buffer, UINT *size)
	{
		if(buffer == NULL || size == NULL)
			return false;
		FILE     *input;
		UINT fileSize, readed;
		const char mode[] = {'r', binary ? 'b' : 't', '\0'};
		fopen_s(&input, fileName.c_str(), mode);
		if(input == NULL)
		{
			return false;
		}
		fseek(input, 0, SEEK_END);
		fileSize = (UINT)ftell(input);
		rewind(input);
		if(fileSize == 0)
		{
			fclose(input);
			return false;
		}
		*buffer = new char[fileSize];
		if(*buffer == NULL)
			return false;
		readed = fread(*buffer, 1, fileSize, input);
		fclose(input);
		/*if (readed != fileSize)
		{
			Error("File::Load", "Reading file '" + fileName + "'\n");
			delete[] *buffer;
			return false;
		}*/
		*size = fileSize;
		return true;
	}

	bool File::Open(const string &name, FILE_TYPE mode)
	{
		if(_file.is_open())
			_file.close();
		_file.open(name, mode);
		return _file.is_open();
	}

	bool File::IsOpen()
	{
		return _file.is_open();
	}

	void File::Close()
	{
		_file.close();
	}

	void File::Write(int value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(int));
	}

	void File::Read(int &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(int));
	}

	void File::Write(char value)
	{
		_file.write(&value, sizeof(char));
	}

	void File::Read(char &value)
	{
		_file.read(&value, sizeof(char));
	}

	void File::Write(bool value)
	{
		char c = char(value);
		_file.write(&c, sizeof(char));
	}

	void File::Read(bool &value)
	{
		char c = 0;
		_file.read(&c, sizeof(char));
		value = (c != 0);
	}

	void File::Write(float value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(float));
	}

	void File::Read(float &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(float));
	}

	void File::Write(UINT value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(UINT));
	}

	void File::Read(UINT &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(UINT));
	}

	void File::Write(UCHAR value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(UCHAR));
	}

	void File::Read(UCHAR &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(UCHAR));
	}

	void File::Write(USHORT value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(USHORT));
	}

	void File::Read(USHORT &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(USHORT));
	}

	void File::Write(const string &value)
	{
		int size = value.size();
		_file.write(reinterpret_cast<char*>(&size), sizeof(int));
		_file.write(value.c_str(), size);
	}

	void File::Read(string &value)
	{
		char c;
		value.clear();
		int i;
		_file.read(reinterpret_cast<char*>(&i), sizeof(int));
		for(int t = 0; t != i; ++t)
		{
			_file.read(&c, 1);
			value.push_back(c);///перекодировать на -c;
		}
	}

	void File::Write(short value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(short));
	}

	void File::Read(short &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(short));
	}

	void File::Write(double value)
	{
		_file.write(reinterpret_cast<char*>(&value), sizeof(double));
	}

	void File::Read(double &value)
	{
		_file.read(reinterpret_cast<char*>(&value), sizeof(double));
	}

	void File::Write(const Vector &value)
	{
		_file.write((char*)&value, sizeof(Vector));
	}

	void File::Read(Vector &value)
	{
		_file.read((char*)&value, sizeof(Vector));
	}

	void File::Write(const Color &value)
	{
		_file.write((char*)&value, sizeof(Color));
	}

	void File::Read(Color &value)
	{
		_file.read((char*)&value, sizeof(Color));
	}

	void File::Write(const ColorRGB &value)
	{
		_file.write((char*)&value, sizeof(ColorRGB));
	}

	void File::Read(ColorRGB &value)
	{
		_file.read((char*)&value, sizeof(ColorRGB));
	}

	void File::Write(const Quaternion &value)
	{
		_file.write((char*)&value, sizeof(Quaternion));
	}

	void File::Read(Quaternion &value)
	{
		_file.read((char*)&value, sizeof(Quaternion));
	}

	void File::Write(const Matrix &value)
	{
		_file.write((char*)&value, sizeof(Matrix));
	}

	void File::Read(Matrix &value)
	{
		_file.read((char*)&value, sizeof(Matrix));
	}

	void File::Write(const Point &value)
	{
		_file.write((char*)&value, sizeof(Point));
	}

	void File::Read(Point &value)
	{
		_file.read((char*)&value, sizeof(Point));
	}

	void File::Write(const Rect &value)
	{
		_file.write((char*)&value, sizeof(Rect));
	}

	void File::Read(Rect &value)
	{
		_file.read((char*)&value, sizeof(Rect));
	}
}