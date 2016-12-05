#include "stdafx.h"
#include "../Core/Variable.h"
#include "Actor.h"
namespace DEN
{
	Actor::Actor()
	{
		_age = 0.0f;
	}
	Actor::~Actor()
	{
		for(auto &v: _vars)
			delete v.second;
		for(auto &v : _coms)
			delete v.second;
	}
};