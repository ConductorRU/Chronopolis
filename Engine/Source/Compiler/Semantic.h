#pragma once
class Syntaxer;
class Compiler;
class Semantic
{
private:
	Compiler *compiler;
	Semantix* _root;
	void Print(Semantix *el, int depth);
	Semantix *Create(Syntax *syn, SEMANTIC_TYPE type);
	Semantix *GetValue(Syntax *syn);
public:
	Semantix *GetRoot() { return _root; };
	void Compile(Syntaxer *syn, Compiler *comp);
	void Print();
};