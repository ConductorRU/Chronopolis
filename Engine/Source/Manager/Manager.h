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
	class Font;
	class Texture;
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
		set<Font*> _fonts;
		set<InputLayout*> _inputs;
		set<Texture*> _texture;
		map<string, Texture*> _textureFile;
		map<string, InputLayout*> _inputDefault;
		static Manager *_this;
	public:
		static Manager *Get() { return _this; }
		static string GetExtension(const string &filename);
		Manager();
		~Manager();
		Mesh *CreateMesh(InputLayout *ia);
		Paramesh *CreateParamesh();
		InputLayout *CreateInputLayout(const string &defaultName = "");
		InputLayout *GetInputLayout(const string &defaultName);
		Pass *CreatePass();
		VertexShader *CreateVS();
		PixelShader *CreatePS();
		GeometryShader *CreateGS();
		HullShader *CreateHS();
		DomainShader *CreateDS();
		Font *LoadFont(const string &family, int height, bool isBold = false, bool isItalic = false);
		Texture *LoadTexture(const string &filename, bool isSprite);
	};
}