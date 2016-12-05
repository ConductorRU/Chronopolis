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
	void Mesh::GenerateUVSphere(uint pOffset, uint uvOffset)
	{
		Vector2 *uv;
		Vector *pos;
		char *v;
		uint size = (uint)z_vertex.size();
		float r, o, f;
		for(uint i = 0; i < size; ++i)
		{
			v = GetVertex(i);
			pos = (Vector*)(&(v[pOffset]));
			uv = (Vector2*)(&(v[uvOffset]));
			r = pos->Length();
			o = acos(pos->y/r);
			f = atan2(pos->x, pos->z);
			uv->x = o;
			uv->y = f;
		}
	}
	BBox Mesh::CalculateBBox(uint pOffset)
	{
		Vector *pos;
		uint size = (uint)z_vertex.size();
		Vector vMin, vMax;
		if(size > 0)
		{
			pos = (Vector*)(&(GetVertex(0)[pOffset]));
			vMin = vMax = *pos;
		}
		for(uint i = 1; i < size; ++i)
		{
			pos = (Vector*)(&(GetVertex(i)[pOffset]));
			if(vMin.x > pos->x)
				vMin.x = pos->x;
			if(vMin.y > pos->y)
				vMin.y = pos->y;
			if(vMin.z > pos->z)
				vMin.z = pos->z;
			if(vMax.x < pos->x)
				vMax.x = pos->x;
			if(vMax.y < pos->y)
				vMax.y = pos->y;
			if(vMax.z < pos->z)
				vMax.z = pos->z;
		}
		_box = BBox(vMin, vMax);
		return _box;
	}
	void Mesh::GenerateNormals(uint pOffset, uint nOffset)
	{
		size_t iCount = z_index.size();
		Vector *p0, *p1, *p2;
		Vector a, b, n;
		for(size_t i = 0; i < iCount; i += 3)
			*(Vector*)(&(GetVertex(z_index[i])[nOffset])) = 0.0f;
		for(size_t i = 0; i < iCount; i += 3)
		{
			p0 = (Vector*)(&(GetVertex(z_index[i])[pOffset]));
			p1 = (Vector*)(&(GetVertex(z_index[i + 1])[pOffset]));
			p2 = (Vector*)(&(GetVertex(z_index[i + 2])[pOffset]));

			a = *p0 - *p1;
			b = *p0 - *p2;
			n = a.Cross(b);
			*(Vector*)(&(GetVertex(z_index[i])[nOffset])) += n;
			*(Vector*)(&(GetVertex(z_index[i + 1])[nOffset])) += n;
			*(Vector*)(&(GetVertex(z_index[i + 2])[nOffset])) += n;
		}
		iCount = z_vertex.size();
		for(size_t i = 0; i < iCount; ++i)
			((Vector*)(&(GetVertex(i)[nOffset])))->Normalize();
	}
	void Mesh::GenerateUVBox(uint pOffset, uint nOffset, uint uvOffset)
	{
		CalculateBBox(pOffset);
		Vector nl = _box.GetSize();
		float len = 1/((nl.x + nl.y + nl.z)/3.0f);
		Vector2 *uv;
		Vector *pos, *nor;
		char *v;
		uint size = (uint)z_vertex.size();
		float ax, ay, az;
		for(uint i = 0; i < size; ++i)
		{
			v = GetVertex(i);
			pos = (Vector*)(&(v[pOffset]));
			nor = (Vector*)(&(v[nOffset]));
			uv = (Vector2*)(&(v[uvOffset]));
			ax = abs(nor->x);
			ay = abs(nor->y);
			az = abs(nor->z);
			if(ax >= ay && ax >= az)
			{
				if(nor->x >= 0)
				{
					uv->x = pos->y*len;
					uv->y = pos->z*len;
				}
				else
				{
					uv->x = pos->y*len;
					uv->y = pos->z*len;
				}
			}
			else if(ay >= ax && ay >= az)
			{
				if(nor->y >= 0)
				{
					uv->x = pos->z*len;
					uv->y = pos->x*len;
				}
				else
				{
					uv->x = pos->z*len;
					uv->y = pos->x*len;
				}
			}
			else
			{
				if(nor->z >= 0)
				{
					uv->x = pos->x*len;
					uv->y = pos->y*len;
				}
				else
				{
					uv->x = pos->x*len;
					uv->y = pos->y*len;
				}
			}
		}
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