#pragma once
class Compiler;
class Lexer
{
private:
	vector<Lexem> _list;
	void AddWord(string &word, uint row, uint col);
public:
	vector<Lexem> &GetList() { return _list; };
	void Compile(const string &code, class Compiler *comp);
	void Print();
};
