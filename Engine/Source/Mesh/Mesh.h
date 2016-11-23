#pragma once
#include "../Math/Math.h"
namespace DEN
{
	enum TYPE_MESH
	{
		MESH_BOX = 0,
	};
	class RenderMesh;
	class InputLayout;
	class Pass;
	class Mesh: public NodeMatrix
	{
	protected:
		RenderMesh *z_buffer;
		Pass *_pass;
		vector<void*> z_vertex;
		vector<UINT> z_index;
		Color z_color;
		char *GenerateVertex(Vector **pos, Vector **nor, Vector **uv, Color **col, int pOffset, int nOffset, int uvOffset, int colOffset);
	public:
		UINT GetVertexCount() { return (UINT)z_vertex.size(); };
		RenderMesh *GetVertexBuffer() { return z_buffer; };
		Pass *GetPass() { return _pass; };
		void SetMaterial(Pass *pass) { _pass = pass; };
		Color GetColor() { return z_color; };
		Mesh(InputLayout *layout);
		~Mesh();
		void SetColor(const Color &col);
		void Generate(TYPE_MESH type, int pOffset = 0, int nOffset = -1, int uvOffset = -1, int colOffset = -1);
		char *CreateVertex();
		char *GetVertex(uint id);
		void AddIndex(UINT num);
		void AddTriangle(UINT v0, UINT v1, UINT v2);
		void AddQuad(UINT v0, UINT v1, UINT v2, UINT v3);
		void Bake();
	};
};