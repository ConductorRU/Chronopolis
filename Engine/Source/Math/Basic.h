#pragma once
namespace DEN
{
	struct Int
	{
		int value;
		Int(int i): value(i) {}
		const Int operator+(const Int& rv) const
		{
			return (value + rv.value);
    }
	};
};