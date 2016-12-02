#pragma once
namespace DEN
{
	class Scene;
	class Parament
	{
	private:
		
	public:
		Parament();
		~Parament();
	};
	class Paramesh
	{
	private:
		Mesh *_mesh;
		uint _oPos;
		uint _oCol;
		uint _oNor;
	public:
		Paramesh();
		Mesh *GetMesh() {return _mesh;};
		void Begin(InputLayout *ia);
		void End();
		uint AddVertex(const Vector &pos);
		uint AddTriangle(const Vector &v0, const Vector &v1, const Vector &v2);
		uint AddQuad(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3);
		void SetPosition(uint num, const Vector &pos);
		void SetColor(uint num, const Color &col);
		void SetColor(const Color &col);
		void SetNormal(uint num, const Vector &pos);
		Mesh *Generate(Scene *scene, InputLayout *ia, int type = 0);
	};
}