#include "stdafx.h"
#include "../Core/Variable.h"
#include "Actor.h"
namespace DEN
{
	Actor::Actor()
	{

	}
	Actor::~Actor()
	{
		for(auto &v: _vars)
			delete v.second;
		for(auto &v : _coms)
			delete v.second;
	}
};