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
		void SetSize(const Vector &size) { _size = size;};
		Box(InputLayout *layout);
		~Box();
		void BeforeRender();
	};
};