#include "stdafx.h"
#include "Error.h"
#include "Struct.h"
#include "Lexer.h"
#include "Syntaxer.h"
#include "Semantic.h"
#include "Coder.h"
#include "Compiler.h"
#include "Lib.h"

void Compiler::AddType(const string &name, int size)
{
	_types[name] = {name, (int)_types.size(), size};
}
void Compiler::Init()
{
	Error::Init();
	lex = new Lexer;
	syn = new Syntaxer;
	sem = new Semantic;
	coder = new Coder;
	_ops.push_back({";", OPERATOR_DELIM, 20});
	_ops.push_back({",", OPERATOR_DELIM, 17, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"+=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"-=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"*=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"/=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"%=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"<<=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({">>=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"&=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"^=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"|=", OPERATOR_BINARY, 15, ASSOC_RIGHT_TO_LEFT});

	_ops.push_back({"||", OPERATOR_BINARY, 14, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"&&", OPERATOR_BINARY, 13, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"|", OPERATOR_BINARY, 12, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"^", OPERATOR_BINARY, 11, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"&", OPERATOR_BINARY, 10, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"==", OPERATOR_BINARY, 9, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"!=", OPERATOR_BINARY, 9, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"<", OPERATOR_BINARY, 8, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({">", OPERATOR_BINARY, 8, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"<=", OPERATOR_BINARY, 8, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({">=", OPERATOR_BINARY, 8, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"<<", OPERATOR_BINARY, 7, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({">>", OPERATOR_BINARY, 7, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"+", OPERATOR_BINARY, 6, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"-", OPERATOR_BINARY, 6, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"*", OPERATOR_BINARY, 5, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"/", OPERATOR_BINARY, 5, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"%", OPERATOR_BINARY, 5, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"->*", OPERATOR_BINARY, 4, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({".*", OPERATOR_BINARY, 4, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"&", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"*", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"!", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"~", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"-", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"+", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"--", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});
	_ops.push_back({"++", OPERATOR_UNARY, 3, ASSOC_RIGHT_TO_LEFT});

	_ops.push_back({"->", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({".", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"]", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"[", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({")", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"(", OPERATOR_BINARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"--", OPERATOR_UNARY, 2, ASSOC_LEFT_TO_RIGHT});
	_ops.push_back({"++", OPERATOR_UNARY, 2, ASSOC_LEFT_TO_RIGHT});

	_ops.push_back({"::", OPERATOR_BINARY, 1, ASSOC_LEFT_TO_RIGHT});

	_keywords.insert("operator");
	AddType("bool", sizeof(bool));
	AddType("char", sizeof(char));
	AddType("uchar", sizeof(unsigned char));
	AddType("short", sizeof(short));
	AddType("ushort", sizeof(unsigned short));
	AddType("int", sizeof(int));
	AddType("uint", sizeof(unsigned int));
	AddType("long", sizeof(long long));
	AddType("ulong", sizeof(unsigned long long));
	AddType("float", sizeof(float));
	AddType("double", sizeof(double));
	Lib::InitFunc(this);
}

void Compiler::AddFunc(const string &func)
{
	lex->Compile(func, this);
	vector<Lexem> lexes = lex->GetList();
	Func *f = new Func;
	f->iReturn = GetType(lexes[0].word);
	size_t i = 1;
	string cl = "";
	if(lexes[2].word == "::")
	{
		f->iClass = GetType(lexes[1].word);
		cl = lexes[1].word;
		i = 3;
	}
	else
		f->iClass = nullptr;
	f->name = lexes[i].word;
	if(f->name == "operator")
		f->name = lexes[++i].word;
	++i;
	_funcs[cl][f->name] = f;
	Type *t = nullptr;
	string tName;
	if(lexes[i].word == "(")
	{
		++i;
		while(i < lexes.size())
		{
			if(lexes[i].word == "," || lexes[i].word == ")")
			{
				Param p;
				p.type = t;
				p.name = tName;
				f->iParams.push_back(p);
				t = nullptr;
				tName.clear();
			}
			else
			{
				Type *x = GetType(lexes[i].word);
				if(x)
					t = x;
				else if(t)
					tName = lexes[i].word;
			}
			++i;
		}
	}
}
Operator *Compiler::GetOperator(const string &val, bool isLeft, bool isRight)
{
	for(Operator &op : _ops)
		if(op.name == val && ((isLeft && isRight && op.type == OPERATOR_BINARY)
			|| (isLeft && !isRight && op.type == OPERATOR_UNARY && ASSOC_RIGHT_TO_LEFT)
			|| (!isLeft && isRight && op.type == OPERATOR_UNARY && ASSOC_LEFT_TO_RIGHT)))
			return &op;
	return nullptr;
}
bool Compiler::IsOperator(const string &val)
{
	for(Operator &op : _ops)
		if(op.name == val)
			return true;
	return false;
}

Type *Compiler::GetType(const string &val)
{
	auto it = _types.find(val);
	if(it == _types.end())
		return nullptr;
	return &it->second;
}
bool Compiler::IsKeyword(const string &val)
{
	auto it = _keywords.find(val);
	if(it == _keywords.end())
		return false;
	return true;
}
bool Compiler::IsWord(const char &c)
{
	if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
		return true;
	return false;
}
bool Compiler::IsNumber(const char &c)
{
	if((c >= '0' && c <= '9'))
		return true;
	return false;
}

bool Compiler::IsNumber(const string &val)
{
	uint s = 0;
	size_t size = val.size();
	if(size > 1 && val[0] == '-')
		s = 1;
	for(size_t i = s; i < size; ++i)
		if(!(val[i] >= '0' && val[i] <= '9'))
			return false;
	return true;
}
bool Compiler::IsVarName(const string &val)
{
	if(IsKeyword(val) || GetType(val) || IsOperator(val))
		return false;
	return true;
}
void Compiler::Compile(const string &code)
{
	lex->Compile(code, this);
	lex->Print();
	cout << endl << "Syntax Tree:" << endl;
	syn->Compile(lex, this);
	syn->Print();
	cout << endl << "Semantic Tree:" << endl;
	sem->Compile(syn, this);
	sem->Print();
	cout << endl << "Coder Tree:" << endl;
	coder->Compile(sem, this);
	coder->Print();
}