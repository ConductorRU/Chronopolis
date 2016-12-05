#pragma once
namespace DEN
{
	class Scene;
	class ParaLayout
	{
	private:
		struct ParaField
		{
			string name;
			uint offset;
		};
		vector<ParaField> z_decl;
		UINT z_size;
	public:
		ParaLayout();
		~ParaLayout();
		UINT GetCount() { return (UINT)z_decl.size(); };
		UINT GetSize() { return z_size; };
		int GetOffset(const string &name);
		void Add(const string &name, uint size);
	};
	class Parament
	{
	private:
		map<string, uint> _connectors;
		map<string, string> _connects;
	public:
		Parament();
		~Parament();
	};
	class Paramesh
	{
	private:
		Mesh *_mesh;
		map<string, uint> _connectors;
		map<string, string> _connects;
		vector<Paramesh*> _templates;
		uint _oPos;
		uint _oCol;
		uint _oNor;
		uint _oUV;
	public:
		Paramesh();
		void Connect(Paramesh *temp, const Matrix &m);
		void AddTemplate(Paramesh *mesh);
		void AddConnector(uint index, const string &name);
		void AddConnect(const string &c1, const string &c2);
		Mesh *GetMesh() {return _mesh;};
		void Begin(InputLayout *ia);
		void End();
		void GenerateNormals();
		void GenerateUV();
		uint AddVertex(const Vector &pos);
		uint AddTriangle(const Vector &v0, const Vector &v1, const Vector &v2);
		uint AddQuad(const Vector &v0, const Vector &v1, const Vector &v2, const Vector &v3);
		void SetPosition(uint num, const Vector &pos);
		void SetColor(uint num, const Color &col);
		void SetColor(const Color &col);
		void SetNormal(uint num, const Vector &pos);
		void SetUV(uint num, const Vector2 &uv);
		Mesh *Generate(Scene *scene, InputLayout *ia, int type = 0);
	};
}