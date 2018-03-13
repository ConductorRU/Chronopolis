#pragma once
namespace DEN
{
	class Vector;
	class Color;
	class ColorRGB;
	class Quaternion;
	class Matrix;
	class Point;
	class Rect;
	enum FILE_TYPE
	{
		FILE_READ = fstream::in,
		FILE_WRITE = fstream::out,
		FILE_APPEND = fstream::app,
		FILE_BINARY = fstream::binary,
		FILE_AT_END = fstream::ate,
		FILE_TRUNCATE = fstream::ate,
	};

	class File
	{
	private:
		fstream _file;
	public:
		File();
		~File();
		bool Load(const string &fileName, bool binary, char **buffer, UINT *size);
		bool Open(const string &name, FILE_TYPE mode);
		bool IsOpen();
		void Close();
		void Write(int value);
		void Read(int &value);
		void Write(char value);
		void Read(char &value);
		void Write(bool value);
		void Read(bool &value);
		void Write(float value);
		void Read(float &value);
		void Write(UINT value);
		void Read(UINT &value);
		void Write(UCHAR value);
		void Read(UCHAR &value);
		void Write(USHORT value);
		void Read(USHORT &value);
		void Write(const string &value);
		void Read(string &value);
		void Write(short value);
		void Read(short &value);
		void Write(double value);
		void Read(double &value);
		void Write(const Vector &value);
		void Read(Vector &value);
		void Write(const Color &value);
		void Read(Color &value);
		void Write(const ColorRGB &value);
		void Read(ColorRGB &value);
		void Write(const Quaternion &value);
		void Read(Quaternion &value);
		void Write(const Matrix &value);
		void Read(Matrix &value);
		void Write(const Point &value);
		void Read(Point &value);
		void Write(const Rect &value);
		void Read(Rect &value);
	};
}