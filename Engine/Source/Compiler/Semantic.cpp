#include "stdafx.h"
#include "Struct.h"
#include "Syntaxer.h"
#include "Semantic.h"
#include "Compiler.h"

CodeVar *CodeBlock::AddVar(const Lexem &name)
{
	CodeVar *v = new CodeVar;
	v->name = name;
	v->id = vars.size();
	vars[name.word] = v;
	return v;
}

Semantix *Semantic::Create(Syntax *syn, SEMANTIC_TYPE type)
{
	Semantix *sem = new Semantix;
	sem->type = type;
	if(syn)
	{
		sem->lexem = syn->value;
		if(syn->type == SYNTAX_TYPENAME)
			sem->typ = syn->typ;
	}
	return sem;
}

Semantix *Semantic::GetValue(Syntax *syn)
{
	Semantix *sem = nullptr;
	if(syn->type == SYNTAX_CONST)
		return Create(syn, SEMANTIC_CONST);
	if(syn->type == SYNTAX_OPERATOR)
	{
		if(syn->op->type == OPERATOR_BINARY)
		{
			Semantix *x = Create(syn, SEMANTIC_FUNCTION);
			x->AddChild(GetValue(syn->childs[0]));
			x->AddChild(GetValue(syn->childs[1]));
			return x;
		}
	}
	return nullptr;
}

void Semantic::Compile(Syntaxer *syn, Compiler *comp)
{
	compiler = comp;
	_root = new Semantix;
	_root->type = SEMANTIC_BLOCK;
	_root->block = new CodeBlock;
	vector<Syntax*> ls = syn->GetList();
	for(Syntax *s : ls)
	{
		if(s->type == SYNTAX_TYPENAME)//объ€влени€ переменных
		{
			Semantix *sem = Create(s, SEMANTIC_INITVAR);
			for(Syntax *vName : s->childs)
			{
				Semantix *vSem = Create(vName, SEMANTIC_VARIABLE);
				CodeVar *var = _root->block->AddVar(vName->value);
				if(var)
				{
					vSem->var = var;
					sem->AddChild(vSem);
					if(vName->childs.size())
					{
						Syntax *vVal = vName->childs[0];
						Semantix *valSem = GetValue(vVal);
						if(valSem)
							vSem->AddChild(valSem);
					}
				}
			}
			_root->childs.push_back(sem);
		}
	}
}

void Semantic::Print(Semantix *el, int depth)
{
	for(int i = 0; i < depth; ++i)
		cout << "  ";
	if(el->type == SEMANTIC_BLOCK)
		cout << "block" << endl;
	else if(el->type == SEMANTIC_INITVAR)
		cout << "init: " << el->typ->name << endl;
	else if(el->lexem.row)
		cout << el->lexem.word << endl;
	else
		cout << "error" << endl;
	for(Semantix *s : el->childs)
		Print(s, depth + 1);
}

void Semantic::Print()
{
	Print(_root, 0);
}