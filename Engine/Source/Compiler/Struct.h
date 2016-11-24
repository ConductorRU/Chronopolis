#pragma once
enum OPERATOR_TYPE
{
	OPERATOR_DELIM = 0,
	OPERATOR_UNARY = 1,
	OPERATOR_BINARY = 2,
	OPERATOR_TERNARY = 3
};
enum ASSOC_TYPE
{
	ASSOC_NONE = 0,
	ASSOC_LEFT_TO_RIGHT = 1,
	ASSOC_RIGHT_TO_LEFT = 2,
};
enum SYNTAX_TYPE
{
	SYNTAX_NONE = 0,
	SYNTAX_KEYWORD = 1,
	SYNTAX_OPERATOR = 2,
	SYNTAX_VARIABLE = 3,
	SYNTAX_BLOCK = 4,
	SYNTAX_TYPENAME = 5,
	SYNTAX_CONST = 6,
};
enum SEMANTIC_TYPE
{
	SEMANTIC_NONE     = 0,
	SEMANTIC_BLOCK    = 1,
	SEMANTIC_INITVAR  = 2,
	SEMANTIC_TYPENAME = 3,
	SEMANTIC_CONST    = 4,
	SEMANTIC_FUNCTION = 5,
	SEMANTIC_VARIABLE = 6,
};
struct Lexem
{
	string word;
	int row;
	int col;
};
struct Operator
{
	string name;
	OPERATOR_TYPE type;
	int prior;
	ASSOC_TYPE assoc;
};
struct Type
{
	string name;
	int id;
	int size;
};
struct Syntax
{
	Syntax *root = nullptr;
	Operator *op = nullptr;
	Type *typ = nullptr;
	vector<Syntax *> childs;
	SYNTAX_TYPE type = SYNTAX_NONE;
	Lexem value;
	void AddChild(Syntax *child) { child->root = this; childs.push_back(child); };
};
struct CodeVar
{
	int id;
	Lexem name;
};
struct CodeBlock
{
	map<string, CodeVar*> vars;
	CodeVar *AddVar(const Lexem &name);
	CodeVar *GetVar(const string &name) { auto it = vars.find(name); if(it != vars.end()) return it->second; return nullptr; };
};
struct Semantix
{
	Semantix *root = nullptr;
	vector<Semantix *> childs;
	SEMANTIC_TYPE type = SEMANTIC_NONE;

	Operator *op = nullptr;
	Type *typ = nullptr;
	CodeVar *var = nullptr;
	CodeBlock *block = nullptr;

	Lexem lexem;
	string cData;
	void AddChild(Semantix *child) { child->root = this; childs.push_back(child);  };
	CodeVar *GetVar(const string &name)
	{
		Semantix *par = this;
		CodeVar *var = nullptr;
		while(par && !var)
		{
			if(block)
				var = block->GetVar(name);
			par = root;
		}
		return var;
	}
};


