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
	public:
		Paramesh();
		void Connect(Paramesh *temp, const Matrix &m);
		void AddTemplate(Paramesh *mesh);
		void AddConnector(uint index, const string &name);
		void AddConnect(const string &c1, const string &c2);
		Mesh *GetMesh() {return _mesh;};
		void Begin(InputLayout *ia);
		void End();
		Mesh *Generate(Scene *scene, InputLayout *ia, int type = 0);
	};
}