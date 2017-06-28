#pragma once
class Lexer;
class Compiler;
struct Type;
class Syntaxer
{
private:
	vector<Syntax*> _root;
	Compiler *compiler;
	Syntax *SplitOp(const vector<Lexem> &block, uint start, uint end, uint &cur);
	Syntax *TypeBlock(const vector<Lexem> &block, Type *type);
	Syntax *CompileBlock(const vector<Lexem> &block);
	void Print(Syntax *el, int depth);
public:
	vector<Syntax*> &GetList() { return _root; };
	void Compile(Lexer *lexer, Compiler *comp);
	void Print();
};