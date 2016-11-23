#pragma once
namespace DEN
{
	class Scene;
	class Parament
	{
	private:

	public:
		Parament();
		~Parament();
	};
	class Paramesh
	{
	public:
		Mesh *Generate(Scene *scene, InputLayout *ia, int type = 0);
	};
}