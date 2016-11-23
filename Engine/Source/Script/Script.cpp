#include "stdafx.h"
#include "Struct.h"
#include "Script.h"

void Lib::AddFunc(const string &name, const string &ret, const string &args, func f)
{
	Function *fun = new Function;
	fun->f = f;
	fun->id = fn2.size();
	fn["void"][name] = fun;
	fn2.push_back(fun);
}
void Lib::Init()
{
	AddFunc("+", "int", "int, int", [](char *ob, char *ret, char *data, int &n)
	{
		int a = *(int*)Lib::v(data, n);
		int b = *(int*)Lib::v(data, n);
		int r = (a + b);
		memcpy(ret, &r, sizeof(int));
	});
}

void Lib::Exec(char *ob, char *ret, char *data, int &n)
{
	fn2[*(int*)&data[n]]->f(ob, ret, data, n);
}

void Script::Init(char *data, int size, int entry)
{
	_data = new char[size];
	memcpy(_data, data, size);
	_entry = entry;
}
void Script::Run()
{
	Lib lib;
	lib.Init();
	_data = new char[24];
	int a = 12, b = 7, r1 = 4, r2 = 8, f = 0;
	int r = 0;
	memcpy(&_data[4], &a, sizeof(a));//data
	memcpy(&_data[8], &b, sizeof(b));
	memcpy(&_data[12], &f, sizeof(f));//code
	memcpy(&_data[16], &r1, sizeof(r1));
	memcpy(&_data[20], &r2, sizeof(r2));
	_entry = 12;
	lib.Exec(nullptr, &_data[0], _data, _entry);
	memcpy(&r, _data, sizeof(r));
}