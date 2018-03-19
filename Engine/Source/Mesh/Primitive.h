#pragma once
#include "../Math/Math.h"
namespace DEN
{
	class Box: public Mesh
	{
	private:
		Vector _size;
		bool _updatePrimitive;
		void Create();
		void Change();
	public:
		Vector GetSize() const {return _size; };
		void SetSize(const Vector &size) { _size = size; _updatePrimitive = true;};
		Box(InputLayout *layout);
		~Box();
		void BeforeRender();
	};

	class Sphere: public Mesh
	{
	private:
		float _radius;
		uint _segments;
		bool _updatePrimitive;
		void Create();
		void Change();
	public:
		float GetSize() const {return _radius; };
		void SetSize(float radius) { _radius = radius; _updatePrimitive = true;};
		uint GetSegments() const {return _segments; };
		void SetSegments(uint segments) { _segments = max(4, segments); _updatePrimitive = true;};
		Sphere(InputLayout *layout);
		~Sphere();
		void BeforeRender();
	};

};