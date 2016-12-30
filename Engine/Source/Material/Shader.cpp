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
		_constSize = 0;
	}
	Shader::~Shader()
	{
		if(_shader)
			_shader->Release();
		if(_pReflector)
			_pReflector->Release();
		for(ShaderConst *c : _valsList)
			delete c;
	}
	void *Shader::GetCode()
	{
		return _shader->GetBufferPointer();
	}
	SIZE_T Shader::GetCodeSize()
	{
		return _shader->GetBufferSize();
	}
	bool Shader::GetConstant(const string &buffer, const string &name, uint &id, uint &offset, uint &size)
	{
		auto &it = _vals.find(buffer);
		if(it == _vals.end())
			return false;
		auto &it2 = it->second->vals.find(name);
		offset = it2->second.ofsset;
		size = it2->second.size;
		id = it->second->id;
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
			info->Release();
			return false;
		}
		Reflect();
		info->Release();
		return true;
	}
	bool Shader::CompileFromFile(const wstring &filename, const string &target, const string &entry, const D3D_SHADER_MACRO* pDefines, ID3DInclude* pInclude)
	{
		ID3D10Blob *info = nullptr;
#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif
		HRESULT hr = D3DCompileFromFile(filename.c_str(), pDefines, pInclude, entry.c_str(), target.c_str(), compileFlags, 0, &_shader, &info);
		if(hr != S_OK)
		{
			info->Release();
			return false;
		}
		Reflect();
		info->Release();
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
		for(uint i = 0; i != desk.ConstantBuffers; ++i)
		{
			hh = _pReflector->GetConstantBufferByIndex(i);
			hh->GetDesc(&pDesc);
			UINT varCount = pDesc.Variables;
			ShaderConst *cn = new ShaderConst();
			cn->id = i;
			cn->size = pDesc.Size;
			for(UINT v = 0; v != varCount; ++v)
			{
				var = hh->GetVariableByIndex(v);
				var->GetDesc(&vDesc);
				cn->vals[vDesc.Name] = {vDesc.StartOffset, vDesc.Size};
				_constSize = vDesc.StartOffset + vDesc.Size;
			}
			_vals[pDesc.Name] = cn;
			_valsList.push_back(cn);
		}
		return true;
	}
	VertexShader::VertexShader()
	{
		_buf = nullptr;
		_type = SHADER_VS;
	}
	VertexShader::~VertexShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool VertexShader::_Compile()
	{
		return HRESULT(Render::Get()->_device->CreateVertexShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf)) == S_OK;
	}
	bool VertexShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMem(data, "vs_" + version, entry))
			return false;
		return _Compile();
	}
	bool VertexShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, "vs_" + version, entry))
			return false;
		return _Compile();
	}
	void *VertexShader::GetBuffer()
	{
		return _buf;
	}
	PixelShader::PixelShader()
	{
		_buf = nullptr;
		_type = SHADER_PS;
	}
	PixelShader::~PixelShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool PixelShader::_Compile()
	{
		return HRESULT(Render::Get()->_device->CreatePixelShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf)) == S_OK;
	}
	bool PixelShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMem(data, "ps_" + version, entry))
			return false;
		return _Compile();
	}
	bool PixelShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, "ps_" + version, entry))
			return false;
		return _Compile();
	}
	void *PixelShader::GetBuffer()
	{
		return _buf;
	}

	GeometryShader::GeometryShader()
	{
		_buf = nullptr;
		_type = SHADER_GS;
	}
	GeometryShader::~GeometryShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool GeometryShader::_Compile()
	{
		return HRESULT(Render::Get()->_device->CreateGeometryShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf)) == S_OK;
	}
	bool GeometryShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMem(data, "gs_" + version, entry))
			return false;
		return _Compile();
	}
	bool GeometryShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, "gs_" + version, entry))
			return false;
		return _Compile();
	}
	void *GeometryShader::GetBuffer()
	{
		return _buf;
	}

	HullShader::HullShader()
	{
		_buf = nullptr;
		_type = SHADER_HS;
	}
	HullShader::~HullShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool HullShader::_Compile()
	{
		return HRESULT(Render::Get()->_device->CreateHullShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf)) == S_OK;
	}
	bool HullShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMem(data, "hs_" + version, entry))
			return false;
		return _Compile();
	}
	bool HullShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, "hs_" + version, entry))
			return false;
		return _Compile();
	}
	void *HullShader::GetBuffer()
	{
		return _buf;
	}

	DomainShader::DomainShader()
	{
		_buf = nullptr;
		_type = SHADER_DS;
	}
	DomainShader::~DomainShader()
	{
		if(_buf)
			_buf->Release();
	}
	bool DomainShader::_Compile()
	{
		return HRESULT(Render::Get()->_device->CreateDomainShader(_shader->GetBufferPointer(), _shader->GetBufferSize(), NULL, &_buf)) == S_OK;
	}
	bool DomainShader::Compile(const string &data, const string &entry, const string &version)
	{
		if(!CompileFromMem(data, "ds_" + version, entry))
			return false;
		return _Compile();
	}
	bool DomainShader::CompileFile(const wstring &filename, const string &entry, const string &version)
	{
		if(!CompileFromFile(filename, "ds_" + version, entry))
			return false;
		return _Compile();
	}
	void *DomainShader::GetBuffer()
	{
		return _buf;
	}
}