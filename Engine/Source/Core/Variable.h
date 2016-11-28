#pragma once
namespace DEN
{
	class Variable
	{
	protected:
		void *val;
		Variable(void *v);
		~Variable();
	public:
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
		void *Get(uint num)
		{
			return vals[num];
		}
	};
	template<typename T>
	class VarList: public Variable
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
		void Clear()
		{
			for(T *v : vals)
				delete v;
			vals.clear();
		}
	};
}