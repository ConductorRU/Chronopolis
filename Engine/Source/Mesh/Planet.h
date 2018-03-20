#pragma once
#include "../Math/Math.h"
namespace DEN
{
	class Planet :public Mesh
	{
	private:

	public:
		Planet(InputLayout *layout);
		~Planet();
		void Generate(uint size, float radius = 1.0f);
	};
};