#include "stdafx.h"
#include "Error.h"
bool Error::isInit = false;
map<int, ErrorDesc> Error::_desc;
void Error::Init()
{
	isInit = true;
}
void Error::Add(int id, const string &text, short wLevel)
{
	_desc[id] = {id, text, wLevel};
}
ErrorDesc *Error::Get(int id)
{
	auto it = _desc.find(id);
	if(it != _desc.end())
		return &it->second;
	return nullptr;
}
bool Error::Print(int line, int col, ERROR_TYPE type, int id, const char *params, ...)
{
	if(!isInit)
		return true;
	ErrorDesc *d = Get(id);
	if(!d)
		return true;
	string s = to_string(line) + ":" + to_string(col) + " ";
	int num = 0;
	if(type == 1)
	{
		num = 1000 + id;
		s += "Lexic error #" + to_string(num);
	}
	if(type == 2)
	{
		num = 2000 + id;
		s += "Syntax error #" + to_string(num);
	}
	if(type == 3)
	{
		num = 3000 + id;
		s += "Semantic error #" + to_string(num);
	}
	if(type == 4)
	{
		num = 4000 + id;
		s += "Codegen error #" + to_string(num);
	}
	s += d->text;
	printf_s(s.c_str(), params);
	if(d->wLevel == 0)
		return true;
	return false;
}