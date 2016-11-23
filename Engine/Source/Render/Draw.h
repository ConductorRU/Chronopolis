#pragma once
namespace DEN
{
	class Scene;
	class Light;
	class Draw
	{
	public:
		virtual void Bake() {};
		virtual void Render(class Scene *scene, class Light *light) {};
	};
}