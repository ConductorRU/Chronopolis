#pragma once
#include "../Core/Variable.h"
namespace DEN
{
	class Variable;
	class ActorScript;
	class Actor
	{
	protected:
		ActorScript *_script;
		uint _seed;
		double _age;//возраст, лет
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
				if(v->second->Get() == var)
					return;
				delete v->second;
				_vars.erase(name);
			}
			Var<T> *vx = new Var<T>(var);
			_vars[name] = vx;
		}
		void *GetVariable(const string &name)
		{
			auto v = _vars.find(name);
			if(v != _vars.end())
				return v->second->Get();
			return nullptr;
		}
		void *GetComponent(const string &name)
		{
			auto v = _coms.find(name);
			if(v != _coms.end())
				return v->second->Get();
			return nullptr;
		}
		ActorScript *GetScript() {return _script;};
		void SetScript(ActorScript *script) {_script = script;};
	};
	class ActorScript
	{
	public:
		map<string, function<void*(char **)>> func;
		Actor *Create();

	};

}