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
		BBox _box;
		RenderMesh *z_buffer;
		Pass *_pass;
		vector<void*> z_vertex;
		vector<UINT> z_index;
		Color z_color;
		uint _oPos;
		uint _oCol;
		uint _oNor;
		uint _oUV;
		bool _isPickable;
		bool _isBaked;
		char *GenerateVertex(Vector **pos, Vector **nor, Vector **uv, Color **col, int pOffset, int nOffset, int uvOffset, int colOffset);
	public:
		bool IsBaked() { return _isBaked; };
		uint GetVertexCount() { return (uint)z_vertex.size(); };
		uint GetIndexCount() { return (uint)z_index.size(); };
		UINT GetIndex(size_t num) { return z_index[num]; };
		RenderMesh *GetVertexBuffer() { return z_buffer; };
		Pass *GetPass() { return _pass; };
		Color GetColor() { return z_color; };
		bool IsPickable() { return _isPickable; };
		void SetPickable(bool pickable) { _isPickable = pickable; };
		Mesh(InputLayout *layout);
		~Mesh();
		void Clear();
		BBox CalculateBBox(uint pOffset);
		void GenerateNormals(uint pOffset, uint nOffset);
		void GenerateUVSphere(uint pOffset, uint uvOffset);
		void GenerateUVBox(uint pOffset, uint nOffset, uint uvOffset);
		void GenerateNormals();
		void GenerateUV();
		void SetMaterial(Pass *pass);
		void SetColor(const Color &col);
		void Generate(TYPE_MESH type, int pOffset = 0, int nOffset = -1, int uvOffset = -1, int colOffset = -1);
		char *CreateVertex();
		char *GetVertex(uint id);
		void AddIndex(UINT num);
		void AddTriangle(UINT v0, UINT v1, UINT v2);
		void AddQuad(UINT v0, UINT v1, UINT v2, UINT v3);
		uint AddVertex(const Vector &pos);
		uint AddTriangle(const Vector &v0, const Vector &v1, const Vector &v2);
		uint AddQuad(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3);
		void SetPosition(uint num, const Vector &pos);
		void SetColor(uint num, const Color &col);
		void SetVertexColor(const Color &col);
		void SetNormal(uint num, const Vector &pos);
		void SetUV(uint num, const Vector2 &uv);
		void Bake();
		bool Pick(const Vector &pos, const Vector &dir, Vector &point);
		virtual void BeforeRender() {};
	};
};