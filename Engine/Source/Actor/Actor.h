#pragma once
namespace DEN
{
	class Variable;
	class Actor
	{
	private:
		vector<Variable*> _vars;
	public:
		template <typename T> void AddVariable(T *var);
	};
}