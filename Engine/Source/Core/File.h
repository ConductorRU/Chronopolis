#pragma once
namespace DEN
{
	enum FILE_TYPE
	{
		FILE_READ = 1,
		FILE_WRITE = 2,
		FILE_APPEND = 3,
		FILE_READ_UPDATE = 4,
		FILE_WRITE_UPDATE = 5,
		FILE_APPEND_UPDATE = 6,
		FILE_TEXT_READ = 7,
		FILE_TEXT_WRITE = 8,
		FILE_TEXT_APPEND = 9,
		FILE_TEXT_READ_UPDATE = 10,
		FILE_TEXT_WRITE_UPDATE = 11,
		FILE_TEXT_APPEND_UPDATE = 12,
	};

	class File
	{
	private:
		FILE *_file;
	public:
		File();
		~File();
		bool Open(const string &filename, FILE_TYPE mode);
		size_t Read(char *c, uint size);
		size_t Write(const char *c, uint size);
		void Close();
	};
}