#pragma once
enum ERROR_TYPE
{
	ERROR_LEXIC = 1,
	ERROR_SYNTAX = 2,
	ERROR_SEMANTIC = 3,
	ERROR_CODEGEN = 4,
};
struct ErrorDesc
{
	int code;
	string text;
	short wLevel;
};
class Error
{
	static bool isInit;
	static map<int, ErrorDesc> _desc;
	static void Add(int id, const string &text, short wLevel);
public:
	static ErrorDesc *Get(int id);
	static void Init();
	static bool Print(int line, int col, ERROR_TYPE type, int id, const char *params, ...);
};