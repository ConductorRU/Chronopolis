#pragma once
namespace DEN
{
	class Format
	{
		
	};
	class ImageFormat
	{
	protected:
		uchar *data;
		uint width;
		uint height;
		uint size;
	public:
		ImageFormat();
		~ImageFormat();
		uint GetWidth() const { return width; }
		uint GetHeight() const { return height; }
		uint GetSize() const { return size; }
		uchar *GetData() const { return data; }
	};
}