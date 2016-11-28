#include "stdafx.h"
#include "../Core/Variable.h"
#include "Actor.h"
namespace DEN
{
	template <typename T> void Actor::AddVariable(T *var)
	{
		Var<T> *v = new Var<T>;
		_vars.push_back(v);
	}
};