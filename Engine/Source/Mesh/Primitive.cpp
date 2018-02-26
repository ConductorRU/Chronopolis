#include "stdafx.h"
#include "Mesh.h"
#include "Primitive.h"
namespace DEN
{
	Box::Box(InputLayout *layout): Mesh(layout)
	{
		_size = Vector::ONE;
		_updatePrimitive = true;
		
	}
	Box::~Box()
	{
	
	}
	void Box::UpdatePrimitive()
	{
		if(!_updatePrimitive)
			return;

		_updatePrimitive = false;
	}
};