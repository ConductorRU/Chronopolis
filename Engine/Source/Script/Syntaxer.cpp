#include "stdafx.h"
#include "Struct.h"
#include "Lexer.h"
#include "Syntaxer.h"
#include "Compiler.h"

Syntax *Syntaxer::SplitOp(const vector<Lexem> &block, int start, int end, int &cur)
{
	int maxPrior = 1000;
	Operator *rootOp = nullptr;
	Lexem lexOp;
	int split = 0;
	for(int i = start; i < end; ++i)
	{
		if(block[i].word == ",")
		{
			end = i;
			cur = i;
			break;
		}
		bool lPar = false, rPar = false;
		if(i > start)
			lPar = compiler->IsVarName(block[i - 1].word);
		if(i + 1< end)
			rPar = compiler->IsVarName(block[i + 1].word);
		Operator *op = compiler->GetOperator(block[i].word, lPar, rPar);
		if(op)
		{
			if(!rootOp || maxPrior < op->prior)
			{
				rootOp = op;
				lexOp = block[i];
				maxPrior = op->prior;
				split = i;
			}
		}
	}
	if(start == end - 1)
	{
		Syntax *par = new Syntax;
		par->value = block[start];
		par->type = SYNTAX_CONST;
		cur = start;
		return par;
	}
	if(rootOp)
	{
		Syntax *par = new Syntax;
		par->value = lexOp;
		par->type = SYNTAX_OPERATOR;
		par->op = rootOp;
		Syntax *c1 = SplitOp(block, start, split, cur);
		Syntax *c2 = SplitOp(block, split + 1, end, cur);
		if(c1)
		{
			c1->root = par;
			par->childs.push_back(c1);
		}
		if(c2)
		{
			c2->root = par;
			par->childs.push_back(c2);
		}
		return par;
	}
	return nullptr;
}

Syntax *Syntaxer::TypeBlock(const vector<Lexem> &block, Type *type)
{
	int size = block.size();
	Syntax *parent = new Syntax;
	parent->type = SYNTAX_TYPENAME;
	parent->value = block[0];
	parent->typ = type;
	bool isComa = false;
	Syntax *var = nullptr;
	for(int i = 1; i < size; ++i)
	{
		if(compiler->IsVarName(block[i].word))//if typename
		{
			var = new Syntax;
			var->value = block[i];
			var->type = SYNTAX_VARIABLE;
			var->root = parent;
			if(var->root)
				var->root->childs.push_back(var);
			isComa = false;
		}
		else if(!isComa && block[i].word == ",")//if comma
		{
			isComa = true;
		}
		else if(!isComa && var && block[i].word == "=")//if setter
		{
			++i;
			Syntax *val = SplitOp(block, i, block.size(), i);
			if(val)
			{
				var->childs.push_back(val);
				val->root = var;
			}
		}
		else
		{
			cout << "Error" << endl;
		}
	}
	return parent;
}
Syntax *Syntaxer::CompileBlock(const vector<Lexem> &block)
{
	int size = block.size();
	if(!size)
		return nullptr;
	Syntax *parent = nullptr;
	Type *type = compiler->GetType(block[0].word);
	if(type)
	{
		parent = TypeBlock(block, type);
	}
	else
	{
		int i = 0;
		parent = SplitOp(block, 0, block.size(), i);
	}
	return parent;
}
void Syntaxer::Compile(Lexer *lexer, class Compiler *comp)
{
	compiler = comp;
	vector<Lexem> list = lexer->GetList();
	int size = list.size();
	string s;
	vector<Lexem> block;
	for(int i = 0; i < size; ++i)
	{
		s = list[i].word;
		if(s != ";")
			block.push_back(list[i]);
		else
		{
			Syntax *par = CompileBlock(block);
			if(par)
				_root.push_back(par);
			block.clear();
		}
	}
}
void Syntaxer::Print(Syntax *el, int depth)
{
	for(int i = 0; i < depth; ++i)
		cout << "  ";
	cout << el->value.word << endl;
	for(Syntax *s : el->childs)
		Print(s, depth + 1);
}
void Syntaxer::Print()
{
	for(Syntax *s : _root)
		Print(s, 0);
}