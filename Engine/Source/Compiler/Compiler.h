#pragma once
class Lexer;
class Syntaxer;
class Semantic;
class Coder;
struct Operator;
struct Type;
struct Func;
class Compiler
{
private:
	Lexer *lex;
	Syntaxer *syn;
	Semantic *sem;
	Coder *coder;
	vector<Operator> _ops;
	set<string> _keywords;
	map<string, Type> _types;
	map<string, map<string, Func*>> _funcs;
public:
	void Init();
	void AddType(const string &name, int size);
	void AddFunc(const string &func);
	Operator *GetOperator(const string &val, bool isLeft, bool isRight);
	bool IsOperator(const string &val);
	Type *GetType(const string &val);
	bool IsKeyword(const string &val);
	bool IsWord(const char &c);
	bool IsNumber(const char &c);
	bool IsNumber(const string &val);
	bool IsVarName(const string &val);
	void Compile(const string &code);
};