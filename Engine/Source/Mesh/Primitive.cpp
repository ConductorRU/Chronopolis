#include "stdafx.h"
#include "Mesh.h"
#include "Primitive.h"
namespace DEN
{
	Box::Box(InputLayout *layout): Mesh(layout)
	{
		_size = Vector::ONE;
		_updatePrimitive = true;
		Create();
	}
	Box::~Box()
	{
	
	}
	void Box::Create()
	{
		for(int i = 0; i < 6; ++i)
			AddQuad(Vector::ZERO, Vector::ZERO, Vector::ZERO, Vector::ZERO);
	}
	void Box::Change()
	{
		Vector c = Vector(0.0f, 0.0f, 0.0f);
		Vector p = _size*0.5f;
		Vector pMin = c - p;
		Vector pMax = c + p;
		
		*((Vector*)GetVertex(0)) = *((Vector*)GetVertex(8)) = *((Vector*)GetVertex(16)) = Vector(pMin.x, pMin.y, pMin.z);
		*((Vector*)GetVertex(1)) = *((Vector*)GetVertex(11)) = *((Vector*)GetVertex(20)) = Vector(pMax.x, pMin.y, pMin.z);
		*((Vector*)GetVertex(2)) = *((Vector*)GetVertex(13)) = *((Vector*)GetVertex(23)) = Vector(pMax.x, pMin.y, pMax.z);
		*((Vector*)GetVertex(3)) = *((Vector*)GetVertex(12)) = *((Vector*)GetVertex(17)) = Vector(pMin.x, pMin.y, pMax.z);
		*((Vector*)GetVertex(4)) = *((Vector*)GetVertex(9)) = *((Vector*)GetVertex(19)) = Vector(pMin.x, pMax.y, pMin.z);
		*((Vector*)GetVertex(5)) = *((Vector*)GetVertex(15)) = *((Vector*)GetVertex(18)) = Vector(pMin.x, pMax.y, pMax.z);
		*((Vector*)GetVertex(6)) = *((Vector*)GetVertex(14)) = *((Vector*)GetVertex(22)) = Vector(pMax.x, pMax.y, pMax.z);
		*((Vector*)GetVertex(7)) = *((Vector*)GetVertex(10)) = *((Vector*)GetVertex(21)) = Vector(pMax.x, pMax.y, pMin.z);
		GenerateNormals();
	}
	void Box::BeforeRender()
	{
		if(!_updatePrimitive)
			return;
		Change();
		
		_isBaked = false;
		_updatePrimitive = false;
	}
};