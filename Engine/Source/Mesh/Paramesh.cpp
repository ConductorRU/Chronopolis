#include "stdafx.h"
#include "Mesh.h"
#include "Paramesh.h"
#include "../Scene/Scene.h"
#include "../Render/InputLayout.h"
#include "../Render/RenderMesh.h"
#include "../Manager/Manager.h"
namespace DEN
{
	ParaLayout::ParaLayout()
	{
		z_size = 0;
	}
	ParaLayout::~ParaLayout()
	{
	}
	int ParaLayout::GetOffset(const string &name)
	{
		for(auto &it : z_decl)
			if(it.name == name)
				return it.offset;
		return -1;
	}
	void ParaLayout::Add(const string &name, uint size)
	{
		ParaField d = {name, z_size};
		z_size += size;
		z_decl.push_back(d);
	}
	Parament::Parament()
	{

	}
	Parament::~Parament()
	{

	}
	void Paramesh::AddConnector(uint index, const string &name)
	{
		_connectors[name] = index;
	}
	void Paramesh::AddConnect(const string &c1, const string &c2)
	{
		_connects[c1] = c2;
		_connects[c2] = c1;
	}
	Paramesh::Paramesh()
	{
		_mesh = nullptr;
	}
	void Paramesh::Connect(Paramesh *temp, const Matrix &m)
	{
		Mesh *tm = temp->_mesh;
		size_t vCount = temp->_mesh->GetVertexCount();
		size_t vmCount = _mesh->GetVertexCount();
		size_t iCount = temp->_mesh->GetIndexCount();
		uint iaSize = _mesh->GetVertexBuffer()->GetInputLayout()->GetSize();
		for(size_t v = 0; v < vCount; ++v)
		{
			char *vert = _mesh->CreateVertex();
			memcpy(vert, tm->GetVertex((uint)v), iaSize);
			*((Vector*)vert) = m.TransformCoord(*((Vector*)vert));
		}
		for(size_t v = 0; v < iCount; ++v)
		{
			_mesh->AddIndex(tm->GetIndex(v) + (uint)vmCount);
		}
	}
	void Paramesh::AddTemplate(Paramesh *mesh)
	{
		_templates.push_back(mesh);
	}
	void Paramesh::Begin(InputLayout *ia)
	{
		_mesh = Manager::Get()->CreateMesh(ia);
		_oPos = ia->GetOffset("POSITION");
		_oCol = ia->GetOffset("COLOR");
		_oNor = ia->GetOffset("NORMAL");
		_oUV = ia->GetOffset("UV");
	}
	void Paramesh::End()
	{
		GenerateUV();
		_mesh->Bake();
	}
	void Paramesh::GenerateNormals()
	{
		_mesh->GenerateNormals(_oPos, _oNor);
	}
	void Paramesh::GenerateUV()
	{
		_mesh->GenerateUVBox(_oPos, _oNor, _oUV);
	}
	uint Paramesh::AddVertex(const Vector &pos)
	{
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = pos;
		return _mesh->GetVertexCount();
	}
	uint Paramesh::AddTriangle(const Vector &v0, const Vector &v1, const Vector &v2)
	{
		uint s = _mesh->GetVertexCount();
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v0;
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v1;
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v2;
		_mesh->AddTriangle(s, s + 1, s + 2);
		return _mesh->GetVertexCount();
	}
	uint Paramesh::AddTriangle(uint v0, uint v1, uint v2)
	{
		_mesh->AddTriangle(v0, v1, v2);
		return _mesh->GetVertexCount();
	}
	uint Paramesh::AddQuad(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3)
	{
		uint s = _mesh->GetVertexCount();
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v0;
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v1;
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v2;
		*((Vector*)(&_mesh->CreateVertex()[_oPos])) = v3;
		if(_oNor)
		{
			Vector a, b, n;
			a = v0 - v1;
			b = v0 - v2;
			n = a.Cross(b);
			SetNormal(s, n);
			SetNormal(s + 1, n);
			SetNormal(s + 2, n);
			SetNormal(s + 3, n);
		}
		if(_oUV)
		{
			SetUV(s, Vector2(0.0f, 0.0f));
			SetUV(s + 1, Vector2(1.0f, 0.0f));
			SetUV(s + 2, Vector2(1.0f, 1.0f));
			SetUV(s + 3, Vector2(0.0f, 1.0f));
		}
		_mesh->AddTriangle(s, s + 1, s + 2);
		_mesh->AddTriangle(s + 2, s + 3, s);
		return _mesh->GetVertexCount();
	}
	void Paramesh::SetPosition(uint num, const Vector &pos)
	{
		*((Vector*)(&_mesh->GetVertex(num)[_oPos])) = pos;
	}
	void Paramesh::SetColor(uint num, const Color &col)
	{
		*((Color*)(&_mesh->GetVertex(num)[_oCol])) = col;
	}
	void Paramesh::SetColor(const Color &col)
	{
		uint s = _mesh->GetVertexCount();
		for(uint i = 0; i < s; ++i)
			SetColor(i, col);
	}
	void Paramesh::SetNormal(uint num, const Vector &nor)
	{
		*((Vector*)(&_mesh->GetVertex(num)[_oNor])) = nor;
	}
	void Paramesh::SetUV(uint num, const Vector2 &uv)
	{
		*((Vector2*)(&_mesh->GetVertex(num)[_oUV])) = uv;
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
			case 1:
			{
				float sizeX = 10.0f, sizeY = 10.0f, sizeZ = 10.0f;
				if(oPos >= 0)
				{
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, 0.0f, sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(sizeX, 0.0f, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, 0.0f, -sizeZ);
					*((Vector*)(&m->CreateVertex()[oPos])) = Vector(-sizeX, 0.0f, sizeZ);
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
						*((Vector*)(&m->GetVertex(i)[oNor])) = Vector(0.0f, 1.0f, 0.0f);
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
		_mesh = m;
		return m;
	}
}