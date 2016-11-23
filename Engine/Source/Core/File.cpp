#include "stdafx.h"
#include "File.h"
namespace DEN
{
	File::File()
	{
		_file = nullptr;
	}
	File::~File()
	{
		if(_file)
			fclose(_file);
	}

	bool File::Open(const string &filename, FILE_TYPE mode)
	{
		Close();
		string m = "";
		switch(mode)
		{
		case FILE_READ: m = "rb"; break;
		case FILE_WRITE: m = "wb"; break;
		case FILE_APPEND: m = "ab"; break;
		case FILE_READ_UPDATE: m = "rb+"; break;
		case FILE_WRITE_UPDATE: m = "wb+"; break;
		case FILE_APPEND_UPDATE: m = "ab+"; break;
		case FILE_TEXT_READ: m = "r"; break;
		case FILE_TEXT_WRITE: m = "w"; break;
		case FILE_TEXT_APPEND: m = "a"; break;
		case FILE_TEXT_READ_UPDATE: m = "r+"; break;
		case FILE_TEXT_WRITE_UPDATE: m = "w+"; break;
		case FILE_TEXT_APPEND_UPDATE: m = "a+"; break;
		}
		if(!fopen_s(&_file, filename.c_str(), m.c_str()))
			return true;
		return false;
	}
	size_t File::Read(char *c, uint size)
	{
		return fread_s(c, 1, 1, 1, _file);
	}
	size_t File::Write(const char *c, uint size)
	{
		return fwrite(c, 1, 1, _file);
	}

	void File::Close()
	{
		if(_file)
			fclose(_file);
		_file = nullptr;
	}
}