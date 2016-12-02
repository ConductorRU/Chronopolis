#pragma once
#include "../Core/Variable.h"
namespace DEN
{
	class Variable;
	class Actor
	{
	protected:
		uint _seed;
		map<string, Variable*> _vars;
		map<string, Variable*> _coms;
	public:
		Actor();
		~Actor();
		template <typename T> void AddVariable(const string &name, T *var)
		{
			Var<T> *v = new Var<T>(var);
			_vars[name] = v;
		}
		template <typename T> void AddComponent(const string &name, T *var)
		{
			Var<T> *v = new Var<T>(var);
			_coms[name] = v;
		}
		template <typename T> void SetVariable(const string &name, T *var)
		{
			auto v = _vars.find(name);
			if(v != _vars.end())
			{
				if(v->second == var)
					return;
				delete v->second;
				_vars.erase(name);
			}
			Var<T> *v = new Var<T>(var);
			_vars[name] = v;
		}
		void *GetVariable(const string &name)
		{
			auto v = _vars.find(name);
			if(v != _vars.end())
				return v->second->Get();
			return nullptr;
		}
	};
}