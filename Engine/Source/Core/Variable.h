#pragma once
namespace DEN
{
	class Variable
	{
	protected:
		void *val;
		//Variable(void *v) {val = v;};
	public:
		virtual ~Variable();
		void *Get() {return val;};
	};
	class VariableList
	{
	protected:
		vector<void*> vals;
		void Add(void *v);
		~VariableList();
	public:
	};
	template<typename T>
	class Var: public Variable
	{
	public:
		Var(T *v)
		{
			val = v;
		}
		~Var()
		{
			delete (T*)val;
		}
		void *Get()
		{
			return vals;
		}
	};
	template<typename T>
	class VarList: public VariableList
	{
	public:
		~VarList()
		{
			for(T *v: vals)
				delete v;
		}
		void Add(T *v)
		{
			vals.push_back(v);
		}
		void *Get(uint num)
		{
			return vals[num];
		}
		void Clear()
		{
			for(T *v : vals)
				delete v;
			vals.clear();
		}
	};
}