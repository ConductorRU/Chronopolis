#include "stdafx.h"
#include "Variable.h"
namespace DEN
{
	Variable::Variable(void *v, size_t h)
	{
		val = v;
		hash = h;
	}
	Variable::~Variable()
	{

	}
}