#include "stdafx.h"
#include "Struct.h"
#include "Semantic.h"
#include "Coder.h"
#include "Compiler.h"
void f1(BYTE *ret, BYTE *data, uint size)
{
	memcpy(ret, data, *(int*)(data + 4));
}
void f2(BYTE *ret, BYTE *data, uint size)
{
	int i = *(int*)data + *(int*)(data + 4);
	memcpy(ret, &i, 4);
}
void f3(BYTE *ret, BYTE *data, uint size)
{
	cout << (*(int*)data);
}
Coder::Coder()
{
	fn.push_back(f1);
	fn.push_back(f1);
	fn.push_back(f2);
	fn.push_back(f3);
}
uint Coder::AddData(CodeVar *var)
{
	var->size = 4;
	var->pos = AddData(var->size);
	return var->pos;
}
uint Coder::AddData(const BYTE *data, uint size)
{
	uint pos = AddData(size);
	memcpy(dataMap + pos, data, size);
	return pos;
}
uint Coder::AddData(uint size)
{
	if(!dataMap)
	{
		dataPos = 4;
		dataSize = 8;
		dataMap = (BYTE*)malloc(dataSize);
	}
	uint pos = dataPos;
	dataPos += size;
	if(dataPos > dataSize)
	{
		while(dataPos > dataSize)
			dataSize *= 2;
		dataMap = (BYTE*)realloc(dataMap, dataSize);
	}
	return pos;
}
uint Coder::AddCode(uint fId, uint retId, const vector<uint> &data)
{
	return AddCode(fId, retId, (BYTE*)(&data[0]), (uint)data.size()*sizeof(uint));
}
uint Coder::FinishCode()
{
	return AddCode(0, 0, nullptr, 4);
}
uint Coder::AddCode(uint fId, uint retId, const BYTE *data, uint size)
{
	if(!codeMap)
	{
		codePos = 4;
		codeSize = 8;
		codeMap = (BYTE*)malloc(codeSize);
	}
	uint siz = sizeof(uint)*3 + size;
	uint pos = codePos;
	if(data == nullptr)
		siz = sizeof(uint);
	codePos += siz;
	if(codePos > codeSize)
	{
		while(codePos > codeSize)
			codeSize *= 2;
		codeMap = (BYTE*)realloc(codeMap, codeSize);
	}
	if(data != nullptr)
	{
		memcpy(codeMap + pos, &siz, 4);
		memcpy(codeMap + pos + 4, &fId, 4);
		memcpy(codeMap + pos + 8, &retId, 4);
		memcpy(codeMap + pos + 12, data, size);
	}
	else
		memset(codeMap + pos, 0, 4);
	return pos;
}
uint Coder::CompileBlock(uint ret, Semantix *block, Compiler *comp)
{
	if(block->type == SEMANTIC_CONST)
	{
		if(comp->IsNumber(block->lexem.word))
		{
			int i = atoi(block->lexem.word.c_str());
			return AddData((BYTE*)&i, sizeof(int));
		}
	}
	if(block->type == SEMANTIC_FUNCTION)
	{
		uint funcId = 2;
		vector<uint> data;
		for(Semantix *ch : block->childs)
			data.push_back(CompileBlock(0, ch, comp));
		AddCode(funcId, ret, data);
	}
	return ret;
}
void Coder::Compile(Semantic *sem, Compiler *comp)
{
	Semantix *root = sem->GetRoot();
	uint pos = 0;
	vector<uint> data;
	for(Semantix *s: root->childs)
	{
		if(s->type == SEMANTIC_INITVAR)
		{
			for(Semantix *var: s->childs)
			{
				pos = AddData(var->var);
				if(var->childs.size())
				{
					Semantix *v = var->childs[0];
					if(v->type == SEMANTIC_CONST)
					{
						if(comp->IsNumber(v->lexem.word))
						{
							int i = atoi(v->lexem.word.c_str());
							pos = AddData((BYTE*)&i, sizeof(int));
						}
					}
					else
					{
						uint p[2] = {CompileBlock(pos, v, comp), 4};
						AddCode(1, pos, (BYTE*)p, 4);
					}
				}
			}
		}
	}
	FinishCode();
}
void Coder::Print()
{
	uint p = 4;
	uint fId, size;
	cout << "Data size:" << " " << dataPos << endl;
	cout << "Code size:" << " " << codePos << endl;
	while(p < codeSize)
	{
		memcpy(&size, codeMap + p, 4);
		if(!size)
			break;
		memcpy(&fId, codeMap + p + 4, 4);
		cout << fId << " " << size << endl;
		p += size;
	}
	Execute();
}
void Coder::Execute()
{
	uint p = 4;
	uint fId, size;
	while(p < codeSize)
	{
		memcpy(&size, codeMap + p, 4);
		if(!size)
			break;
		memcpy(&fId, codeMap + p + 4, 4);
		fn[fId](codeMap + p + 8, codeMap + p + 12, size - 12);
		p += size;
	}
}