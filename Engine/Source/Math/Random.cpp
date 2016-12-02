#include "stdafx.h"
#include "Random.h"
namespace DEN
{
	Random::Random(uint s)
	{
		seed = s;
	}

	void Random::Generate(uint count)
	{
		srand(seed);
		for(uint i = 0; i < count; ++i)
			nums.push_back(rand());
	}
}