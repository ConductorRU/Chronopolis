#pragma once
typedef void (*func)(char *ob, char *ret, char *data, int &n);
struct Function
{
	func f;
	int id;
};
class Lib
{
public:
	map<string, map<string, Function*>> fn;
	vector<Function *> fn2;
	static void *v(char *data, int &n) { return &data[*(int*)&data[n += 4]]; }
	void AddFunc(const string &name, const string &ret, const string &args, func f);
	void Exec(char *ob, char *ret, char *data, int &n);
	void Init();
};

class Script
{
	char *_data;
	int _entry;
public:
	void Init(char *data, int size, int entry);
	void Run();
};