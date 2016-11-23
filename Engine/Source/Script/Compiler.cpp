#include "stdafx.h"
#include "Error.h"
#include "Struct.h"
#include "Lexer.h"
#include "Syntaxer.h"
#include "Semantic.h"
#include "Coder.h"
#include "Compiler.h"

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