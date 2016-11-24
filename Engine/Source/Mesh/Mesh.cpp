#include "stdafx.h"
#include "../Render/Render.h"
#include "../Material/Shader.h"
#include "../Material/Pass.h"
#include "Mesh.h"
namespace DEN
{
	Mesh::Mesh(InputLayout *layout)
	{
		z_buffer = new RenderMesh(layout);
		_pass = nullptr;
	}
	Mesh::~Mesh()
	{
		delete z_buffer;
	}
	void Mesh::SetMaterial(Pass *pass)
	{
		_pass = pass;
		GetVertexBuffer()->Init(pass);
	};
	void Mesh::SetColor(const Color &col)
	{
		z_color = col;
	}
	char *Mesh::GenerateVertex(Vector **pos, Vector **nor, Vector **uv, Color **col, int pOffset, int nOffset, int uvOffset, int colOffset)
	{
		char *v = CreateVertex();
		if(pOffset >= 0)
			*pos = (Vector*)(&v[pOffset]);
		if(nOffset >= 0)
			*nor = (Vector*)(&v[nOffset]);
		if(uvOffset >= 0)
			*uv = (Vector*)(&v[uvOffset]);
		if(colOffset >= 0)
			*col = (Color*)(&v[colOffset]);
		return v;
	}
	void Mesh::Generate(TYPE_MESH type, int pOffset, int nOffset, int uvOffset, int colOffset)
	{
		Vector *pos = nullptr;
		Vector *nor = nullptr;
		Vector *uv = nullptr;
		Color *col = nullptr;
		char *v;
		if(type == MESH_BOX)
		{
			vector<Vector*> vPos;
			vector<Vector*> vNor;
			vector<Vector*> vUV;
			vector<Color*> vCol;
			for(UINT i = 0; i != 48; ++i)
			{
				v = GenerateVertex(&pos, &nor, &uv, &col, pOffset, nOffset, uvOffset, colOffset);
				if(pos)
					vPos.push_back(pos);
				if(nor)
					vNor.push_back(nor);
				if(uv)
					vUV.push_back(uv);
				if(col)
				{
					vCol.push_back(col);
					*col = Color(1.0f*(i == 0), 1.0f*(i == 1), 1.0f*(i == 2), 1.0f);
				}
			}
			float s = 1.0f;

			*vPos[0] = Vector(-s, -s, -s);
			*vPos[1] = Vector(s, -s, -s);
			*vPos[2] = Vector(s, -s, s);
			*vPos[3] = Vector(-s, -s, s);

			*vPos[4] = Vector(-s, -s, -s);
			*vPos[5] = Vector(-s, s, -s);
			*vPos[6] = Vector(s, s, -s);
			*vPos[7] = Vector(s, -s, -s);

			*vPos[8] = Vector(-s, -s, -s);
			*vPos[9] = Vector(-s, -s, s);
			*vPos[10] = Vector(-s, s, s);
			*vPos[11] = Vector(-s, s, -s);

			*vPos[12] = Vector(-s, s, -s);
			*vPos[13] = Vector(-s, s, s);
			*vPos[14] = Vector(s, s, s);
			*vPos[15] = Vector(s, s, -s);

			*vPos[16] = Vector(s, -s, -s);
			*vPos[17] = Vector(s, s, -s);
			*vPos[18] = Vector(s, s, s);
			*vPos[19] = Vector(s, -s, s);

			*vPos[20] = Vector(s, -s, s);
			*vPos[21] = Vector(s, s, s);
			*vPos[22] = Vector(-s, s, s);
			*vPos[23] = Vector(-s, -s, s);

			for(UINT i = 0; i != 6; ++i)
				AddQuad(0 + i * 4, 1 + i * 4, 2 + i * 4, 3 + i * 4);
			if(vNor.size())
			{
				*vNor[0] = *vNor[1] = *vNor[2] = *vNor[3] = Vector(0.0f, -1.0f, 0.0f);
				*vNor[4] = *vNor[5] = *vNor[6] = *vNor[7] = Vector(0.0f, 0.0f, -1.0f);
				*vNor[8] = *vNor[9] = *vNor[10] = *vNor[11] = Vector(-1.0f, 0.0f, 0.0f);
				*vNor[12] = *vNor[13] = *vNor[14] = *vNor[15] = Vector(0.0f, 1.0f, 0.0f);
				*vNor[16] = *vNor[17] = *vNor[18] = *vNor[19] = Vector(1.0f, 0.0f, 0.0f);
				*vNor[20] = *vNor[21] = *vNor[22] = *vNor[23] = Vector(0.0f, 0.0f, 1.0f);
			}
			/**vPos[0] = Vector(-s, -s, -s);
			*vPos[1] = Vector(-s, -s, s);
			*vPos[2] = Vector(s, -s, s);
			*vPos[3] = Vector(s, -s, -s);
			*vPos[4] = Vector(-s, s, -s);
			*vPos[5] = Vector(-s, s, s);
			*vPos[6] = Vector(s, s, s);
			*vPos[7] = Vector(s, s, -s);
			AddQuad(0, 3, 2, 1);
			AddQuad(0, 4, 7, 3);
			AddQuad(0, 1, 5, 4);
			AddQuad(4, 5, 6, 7);
			AddQuad(3, 7, 6, 2);
			AddQuad(2, 6, 5, 1);

			AddQuad(1, 2, 3, 0);
			AddQuad(3, 7, 4, 0);
			AddQuad(4, 5, 1, 0);

			AddQuad(7, 6, 5, 4);
			AddQuad(2, 6, 7, 3);
			AddQuad(1, 5, 6, 2);*/
		}

	}
	char *Mesh::CreateVertex()
	{
		InputLayout *layout = z_buffer->GetInputLayout();
		if(!layout || !layout->GetSize())
			return nullptr;
		char *v = new char[layout->GetSize()];
		memset(v, 0, layout->GetSize());
		z_vertex.push_back(v);
		return v;
	}
	char *Mesh::GetVertex(uint id)
	{
		return (char*)(z_vertex[id]);
	}
	void Mesh::AddIndex(UINT num)
	{
		z_index.push_back(num);
	}
	void Mesh::AddTriangle(UINT v0, UINT v1, UINT v2)
	{
		z_index.push_back(v0);
		z_index.push_back(v1);
		z_index.push_back(v2);
	}
	void Mesh::AddQuad(UINT v0, UINT v1, UINT v2, UINT v3)
	{
		z_index.push_back(v0);
		z_index.push_back(v1);
		z_index.push_back(v2);
		z_index.push_back(v2);
		z_index.push_back(v3);
		z_index.push_back(v0);
	}
	void Mesh::Bake()
	{
		InputLayout *layout = z_buffer->GetInputLayout();
		if(!z_vertex.size() || !layout || !layout->GetSize())
			return;
		UINT lSize = (UINT)layout->GetSize();
		UINT vSize = (UINT)z_vertex.size();
		UINT pos = 0;
		char *c = new char[lSize*z_vertex.size()];
		for(void *v : z_vertex)
		{
			memcpy(&c[pos], v, lSize);
			pos += lSize;
		}
		z_buffer->Bake(c, vSize, lSize);
		if(z_index.size())
			z_buffer->BakeIndex(&z_index[0], (UINT)z_index.size());
	}
};