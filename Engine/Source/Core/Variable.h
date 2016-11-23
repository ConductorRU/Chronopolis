#pragma once
namespace DEN
{
	class Variable
	{
		size_t hash;
		void *val;
	public:
		Variable(void *v, size_t hash);
		~Variable();
	};
}