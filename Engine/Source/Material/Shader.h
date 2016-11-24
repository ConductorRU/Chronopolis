#pragma once
namespace DEN
{
	enum SHADER_TYPE
	{
		SHADER_VS = 0,
		SHADER_PS = 1,
		SHADER_GS = 2,
		SHADER_HS = 3,
		SHADER_DS = 4,
	};
	struct ShaderValue
	{
		uint ofsset;
		uint size;
	};
	struct ShaderConst
	{
		uint id;
		map<string, ShaderValue> vals;
		uint size;
	};
	class Shader
	{
	protected:
		SHADER_TYPE _type;
		ID3DBlob *_shader;
		ID3D11ShaderReflection* _pReflector;
		map<string, ShaderConst*> _vals;
		vector<ShaderConst*> _valsList;
		uint _constSize;
		bool Reflect();
		bool CompileFromMem(const string &data, const string &target, const string &entry, const D3D_SHADER_MACRO* pDefines = NULL, ID3DInclude* pInclude = NULL);
		bool CompileFromFile(const wstring &filename, const string &target, const string &entry, const D3D_SHADER_MACRO* pDefines = NULL, ID3DInclude* pInclude = NULL);
	public:
		ShaderConst *GetConstant(uint num) { return _valsList[num]; };
		uint GetConstantCount() { return (uint)_valsList.size(); };
		void *GetCode();
		SHADER_TYPE GetType() { return _type; };
		SIZE_T GetCodeSize();
		bool GetConstant(const string &buffer, const string &name, uint &id, uint &offset, uint &size);
		Shader();
		~Shader();
	};
	class VertexShader: public Shader
	{
	private:
		ID3D11VertexShader *_buf;
		bool _Compile();
	public:
		VertexShader();
		~VertexShader();
		void *GetBuffer();
		bool Compile(const string &data, const string &entry = "main", const string &version = "4_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "4_0");
	};
	class PixelShader: public Shader
	{
	private:
		ID3D11PixelShader *_buf;
		bool _Compile();
	public:
		PixelShader();
		~PixelShader();
		void *GetBuffer();
		bool Compile(const string &data, const string &entry = "main", const string &version = "4_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "4_0");
	};
	class GeometryShader: public Shader
	{
	private:
		ID3D11GeometryShader *_buf;
		bool _Compile();
	public:
		GeometryShader();
		~GeometryShader();
		void *GetBuffer();
		bool Compile(const string &data, const string &entry = "main", const string &version = "4_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "4_0");
	};
	class HullShader: public Shader
	{
	private:
		ID3D11HullShader *_buf;
		bool _Compile();
	public:
		HullShader();
		~HullShader();
		void *GetBuffer();
		bool Compile(const string &data, const string &entry = "main", const string &version = "4_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "4_0");
	};
	class DomainShader: public Shader
	{
	private:
		ID3D11DomainShader *_buf;
		bool _Compile();
	public:
		DomainShader();
		~DomainShader();
		void *GetBuffer();
		bool Compile(const string &data, const string &entry = "main", const string &version = "4_0");
		bool CompileFile(const wstring &filename, const string &entry = "main", const string &version = "4_0");
	};
}