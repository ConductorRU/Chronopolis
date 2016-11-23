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
	class Shader
	{
	protected:

		ID3DBlob *_shader;
		ID3D11ShaderReflection* _pReflector;
		map<string, ShaderValue> _vals;
		bool Reflect();
		bool CompileFromMem(const string &data, const string &target, const string &entry, const D3D_SHADER_MACRO* pDefines = NULL, ID3DInclude* pInclude = NULL);
		bool CompileFromFile(const wstring &name, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude, const string &entry, const string &target);
	public:
		void *GetCode();
		UINT GetCodeSize();
		bool GetConstant(const string &name, uint &offset, uint &size);
		Shader();
		~Shader();
	};
	class VertexShader: public Shader
	{
	private:
		ID3D11VertexShader *_buf;
	public:
		VertexShader();
		~VertexShader();
		void *GetBuffer();
		bool Compile(const string &data);

	};
	class PixelShader: public Shader
	{
	private:
		ID3D11PixelShader *_buf;
	public:
		PixelShader();
		~PixelShader();
		void *GetBuffer();
		bool Compile(const string &data);

	};
}