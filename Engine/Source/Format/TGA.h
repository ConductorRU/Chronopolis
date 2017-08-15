#pragma once
namespace DEN
{
	class TGA
	{
	private:
		#pragma pack(push,1)
		typedef struct PNGHeader
		{
		} BMPHeader;
		#pragma pack(pop)
		uchar *data;
		uint width;
		uint height;
		uint size;
	public:
		TGA();
		~TGA();
		uint GetWidth() const { return width; }
		uint GetHeight() const { return height; }
		uint GetSize() const { return size; }
		uchar *GetData() const { return data; }
		static TGA *Load(const string &filename);
		static bool Save(const string &filename, uchar *data, uint sixeX, uint sizeY);
	};
}