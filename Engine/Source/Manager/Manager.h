#pragma once
namespace DEN
{
	class Mesh;
	class VertexShader;
	class PixelShader;
	class GeometryShader;
	class HullShader;
	class DomainShader;
	class InputLayout;
	class Pass;
	class Paramesh;
	class InputLayout;

	class Manager
	{
	private:
		set<Mesh*> _meshes;
		set<VertexShader*> _vs;
		set<PixelShader*> _ps;
		set<GeometryShader*> _gs;
		set<HullShader*> _hs;
		set<DomainShader*> _ds;
		set<Pass*> _passes;
		set<Paramesh*> _paras;
		set<InputLayout*> _inputs;
		static Manager *_this;
	public:
		static Manager *Get() { return _this; }
		Manager();
		~Manager();
		Mesh *CreateMesh(InputLayout *ia);
		Paramesh *CreateParamesh();
		InputLayout *CreateInputLayout();
		Pass *CreatePass();
		VertexShader *CreateVS();
		PixelShader *CreatePS();
		GeometryShader *CreateGS();
		HullShader *CreateHS();
		DomainShader *CreateDS();
	};
}