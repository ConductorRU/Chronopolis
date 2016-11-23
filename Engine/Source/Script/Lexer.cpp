#include "stdafx.h"
#include "Struct.h"
#include "Lexer.h"
#include "Compiler.h"
void Lexer::AddWord(string &word, int row, int col)
{
	if(word.size())
		_list.push_back({word, row, col});
	word.clear();
}
void Lexer::Compile(const string &code, Compiler *comp)
{
	string w;
	int size = code.size();
	char c;
	string s1, s2, s3;
	bool isLast = false;
	int col = 1;
	for(int i = 0; i < size; ++i)
	{
		c = code[i];
		if(i + 1 == size)
			isLast = true;
		s1.clear();
		s2.clear();
		s3.clear();
		s1.push_back(c);
		if(!isLast)
		{
			s2.push_back(c);
			s2.push_back(code[i + 1]);
		}
		if(i + 2 < size)
		{
			s3.push_back(c);
			s3.push_back(code[i + 1]);
			s3.push_back(code[i + 2]);
		}
		if(comp->IsOperator(s3))
		{
			AddWord(w, col, i);
			AddWord(s3, col, i);
			i += 2;
			w.clear();
		}
		else if(comp->IsOperator(s2))
		{
			AddWord(w, col, i);
			AddWord(s2, col, i);
			i += 1;
			w.clear();
		}
		else if(comp->IsOperator(s1))
		{
			AddWord(w, col, i);
			AddWord(s1, col, i);
			w.clear();
		}
		else
		{

			if(c != ' ')
				w += c;
			if(c == ' ' || isLast)
				AddWord(w, col, i);
		}
	}
}

void Lexer::Print()
{
	for(Lexem &s : _list)
		cout << s.word << endl;
}