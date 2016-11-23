#include "stdafx.h"
#include "Shader.h"
#include "../Render/InputLayout.h"
#include "../Render/Direct3d11.h"
namespace DEN
{
	Shader::Shader()
	{
		_shader = nullptr;
		_pReflector = nullptr;
	}
	Shader::~Shader()
	{
		if(_shader)
			_shader->Release();
		if(_pReflector)
			_pReflector->Release();
	}
	void *Shader::GetCode()
	{
		return _shader->GetBufferPointer();
	}
	UINT Shader::GetCodeSize()
	{
		return _shader->GetBufferSize();
	}
	bool Shader::GetConstant(const string &name, uint &offset, uint &size)
	{
		map<string, ShaderValue>::const_iterator it = _vals.find(name);
		if(it == _vals.end())
			return false;
		offset = it->second.ofsset;
		size = it->second.size;
		return true;
	}
	bool Shader::CompileFromMem(const string &data, const string &target, const string &entry, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude)
	{
		ID3D10Blob *info = nullptr;
#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		HRESULT hr = D3DCompile(data.c_str(), data.size(), NULL, pDefines, pInclude, entry.c_str(), target.c_str(), compileFlags, 0, &_shader, &info);
		if(hr != S_OK)
		{
			OutputDebugStringA((char*)info->GetBufferPointer());
			return false;
		}
		Reflect();
		return true;
	}
	bool Shader::CompileFromFile(const wstring &name, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude, const string &entry, const string &target)
	{
		ID3D10Blob *info = nullptr;
#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		HRESULT hr = D3DCompileFromFile(name.c_str(), pDefines, pInclude, entry.c_str(), target.c_str(), compileFlags, 0, &_shader, &info);
		if(hr != S_OK)
			return false;
		Reflect();
		return true;
	}
	bool Shader::Reflect()
	{
		HRESULT hr = D3DReflect(_shader->GetBufferPointer(), _shader->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&_pReflector);
		if(hr != S_OK)
			return false;
		D3D11_SHADER_DESC desk;
		_pReflector->GetDesc(&desk);
		ID3D11ShaderReflectionConstantBuffer *hh = nullptr;
		ID3D11ShaderReflectionVariable *var = nullptr;
		D3D11_SHADER_BUFFER_DESC pDesc;
		D3D11_SHADER_VARIABLE_DESC vDesc;
		uint rSize = 0;
		for(UINT i = 0; i != desk.ConstantBuffers; ++i)
		{
			hh = _pReflector->GetConstantBufferByIndex(i);
			hh->GetDesc(&pDesc);
			UINT varCount = pDesc.Variables;
			for(UINT v = 0; v != varCount; ++v)
			{
				var = hh->GetVariableByIndex(v);
				var->GetDesc(&vDesc);
				_vals[vDesc.Name] = {vDesc.StartOffset, vDesc.Size};
			}
		}
		return true;
	}
	VertexShader::VertexShader()
	{
		_buf = nullptr;
	}
	VertexShader::~VertexShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool VertexShader::Compile(const string &data)
	{
		if(!CompileFromMem(data, "vs_5_0", "main"))
			return false;
		HRESULT hr = Render::Get()->_device->CreateVertexShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf);
		if(hr != S_OK)
			return false;
		return true;
	}
	void *VertexShader::GetBuffer()
	{
		return _buf;
	}
	PixelShader::PixelShader()
	{
		_buf = nullptr;
	}
	PixelShader::~PixelShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool PixelShader::Compile(const string &data)
	{
		if(!CompileFromMem(data, "ps_5_0", "main"))
			return false;
		HRESULT hr = Render::Get()->_device->CreatePixelShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf);
		if(hr != S_OK)
			return false;
		return true;
	}
	void *PixelShader::GetBuffer()
	{
		return _buf;
	}
}