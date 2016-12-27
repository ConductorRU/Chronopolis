#include "stdafx.h"
#include "../Core/Variable.h"
#include "Actor.h"
namespace DEN
{
	Actor::Actor()
	{
		_age = 0.0f;
		_script = nullptr;
	}
	Actor::~Actor()
	{
		for(auto &v: _vars)
			delete v.second;
		for(auto &v : _coms)
			delete v.second;
	}
	Actor *ActorScript::Create()
	{
		auto it = func.find("onCreate");
		if(it == func.end())
			return nullptr;
		void *v = it->second(nullptr);
		return (Actor*)v;
	}
};