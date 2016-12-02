#pragma once
namespace DEN
{
	struct Random
	{
		uint seed;
		vector<uint> nums;
		Random(uint s);
		void Generate(uint count);
	};
}