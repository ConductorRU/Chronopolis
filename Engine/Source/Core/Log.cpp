#include "stdafx.h"
#include "Log.h"
namespace DEN
{
	Log *Log::_this = nullptr;
	Log::Log()
	{
		_this = this;
	}
	Log::~Log()
	{

	}
	void Log::Error(const string &title, const string &message)
	{
		MessageBoxA(NULL, (LPCSTR)message.c_str(), (LPCSTR)title.c_str(), MB_OK);
		exit(1);
	}
}