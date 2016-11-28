#include "stdafx.h"
#include "Variable.h"
namespace DEN
{
	Variable::Variable(void *v)
	{
		val = v;
	}
	Variable::~Variable()
	{

	}
	VariableList::~VariableList()
	{

	}
	void VariableList::Add(void *v)
	{
		vals.push_back(v);
	}
}