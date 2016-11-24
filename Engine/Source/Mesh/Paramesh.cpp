#include "stdafx.h"
#include "Mesh.h"
#include "Paramesh.h"
#include "../Scene/Scene.h"
#include "../Render/InputLayout.h"
#include "../Render/RenderMesh.h"
#include "../Manager/Manager.h"
namespace DEN
{
	Parament::Parament()
	{

	}
	Parament::~Parament()
	{

	}
	Mesh *Paramesh::Generate(Scene *scene, InputLayout *ia, int type)
	{
		Mesh *m = Manager::Get()->CreateMesh(ia);
		scene->AddMesh(m);
		int oPos = ia->GetOffset("POSITION");
		int oColor = ia->GetOffset("COLOR");
		int oNor = ia->GetOffset("NORMAL");
		switch(type)
		{
			case 0://box
			{
				float sizeX = 0.2f, sizeY = 0.2f, sizeZ = 0.2f;
				if(oPos >= 0)
				{
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, sizeZ);
					
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, -sizeZ);

					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, sizeZ);

					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, -sizeZ);
					
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, -sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, sizeY, -sizeZ);
					
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, -sizeY, sizeZ);
				}
				uint cnt = m->GetVertexCount();
				if(oColor >= 0)
				{
					Color col = Color(0.0f, 0.8f, 0.4f, 1.0f);
					for(uint i = 0; i < cnt; ++i)
						*((Color*)(&m->GetVertex(i)[oColor])) = col;
				}
				if(oNor >= 0)
				{
					for(uint i = 0; i < 4; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(0.0f, 0.0f, 1.0f);
					for(uint i = 4; i < 8; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(0.0f, 0.0f, -1.0f);
					for(uint i = 8; i < 12; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(0.0f, 1.0f, 0.0f);
					for(uint i = 12; i < 16; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(0.0f, -1.0f, 0.0f);
					for(uint i = 16; i < 20; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(1.0f, 0.0f, 0.0f);
					for(uint i = 20; i < 24; ++i)
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(-1.0f, 0.0f, 0.0f);
				}
				for(uint i = 0; i < cnt; i += 4)
				{
					m->AddTriangle(i, i + 1, i + 2);
					m->AddTriangle(i + 2, i + 3, i);
				}
				m->Bake();
				break;
			}
		}
		return m;
	}
}