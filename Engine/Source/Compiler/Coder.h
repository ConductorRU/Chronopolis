#pragma once
typedef void (*fun)(BYTE *ret, BYTE *data, uint size);
class Coder
{
private:
	BYTE *dataMap = nullptr;
	uint dataPos = 0;
	uint dataSize = 0;
	BYTE *codeMap = nullptr;
	uint codePos = 0;
	uint codeSize = 0;
	vector<fun> fn;
public:
	Coder();
	uint AddData(CodeVar *block);
	uint AddData(uint size);
	uint AddData(const BYTE *data, uint size);
	uint AddCode(uint fId, uint retId, const vector<uint> &data);
	uint AddCode(uint fId, uint retId, const BYTE *data, uint size);
	uint FinishCode();
	uint CompileBlock(uint ret, Semantix *block, Compiler *comp);
	void Compile(Semantic *sem, Compiler *comp);
	void Print();
	void Execute();
};