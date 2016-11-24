#include "stdafx.h"
#include "ConstantBuffer.h"
#include "../Material/Pass.h"
#include "Render.h"
namespace DEN
{
	ConstantBuffer::ConstantBuffer()
	{
		_updPass = false;
		_pass = nullptr;
		memset(_shaders, 0, sizeof(_shaders));
		memset(_upd, 0, sizeof(_upd));
	}
	ConstantBuffer::~ConstantBuffer()
	{
		Clear();
	}
	void ConstantBuffer::Init(Pass *pass)
	{
		_pass = pass;
		_updPass = true;
	}
	void ConstantBuffer::Init(VertexShader *sh)
	{
		_shaders[SHADER_VS] = sh;
		_upd[SHADER_VS] = true;
	}
	void ConstantBuffer::Init(PixelShader *sh)
	{
		_shaders[SHADER_PS] = sh;
		_upd[SHADER_VS] = true;
	}
	void ConstantBuffer::Init(GeometryShader *sh)
	{
		_shaders[SHADER_GS] = sh;
		_upd[SHADER_VS] = true;
	}
	void ConstantBuffer::Init(HullShader *sh)
	{
		_shaders[SHADER_HS] = sh;
		_upd[SHADER_VS] = true;
	}
	void ConstantBuffer::Init(DomainShader *sh)
	{
		_shaders[SHADER_DS] = sh;
		_upd[SHADER_VS] = true;
	}
	void ConstantBuffer::ReleaseConst(SHADER_TYPE type)
	{
		if(!_buffers[type].size())
			return;
		for(auto &b : _buffers[type])
		{
			b.buf->Release();
			delete b.data;
		}
		_buffers[type].clear();
	}
	bool ConstantBuffer::UpdateConst(Shader *sh)
	{
		ReleaseConst(sh->GetType());
		uint count = sh->GetConstantCount();
		for(uint i = 0; i < count; ++i)
		{
			ShaderConst *c = sh->GetConstant(i);
			D3D11_BUFFER_DESC desc = {0};
			desc.ByteWidth = c->size;
			desc.CPUAccessFlags = 0;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.MiscFlags = 0;
			desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			desc.StructureByteStride = 0;
			Buffer buf = {0};
			buf.data = new UCHAR[c->size];
			buf.size = c->size;
			buf.index = i;
			HRESULT hr = Render::Get()->_device->CreateBuffer(&desc, 0, &buf.buf);
			if(hr != S_OK)
				return false;
			_buffers[sh->GetType()].push_back(buf);
		}
		return true;
	}
	bool ConstantBuffer::UpdateConst()
	{
		if(!_pass)
		{
			for(int i = 0; i < 5; ++i)
				if(_upd[i])
				{
					_upd[i] = false;
					UpdateConst(_shaders[i]);
				}
		}
		else if(_updPass)
		{
			_updPass = false;
			if(_pass->GetVS())
				UpdateConst(_pass->GetVS());
			if(_pass->GetPS())
				UpdateConst(_pass->GetPS());
			if(_pass->GetGS())
				UpdateConst(_pass->GetGS());
			if(_pass->GetHS())
				UpdateConst(_pass->GetHS());
			if(_pass->GetDS())
				UpdateConst(_pass->GetDS());
		}
		return true;
	}
	void ConstantBuffer::Map()
	{

	}
	void ConstantBuffer::Unmap()
	{
		SHADER_TYPE type;
		for(int n = 0; n < 5; ++n)
		{
			type = (SHADER_TYPE)n;
			uint count = (uint)_buffers[n].size();
			for(uint i = 0; i < count; ++i)
			{
				if(_buffers[n][i].isChange)
				{
					_buffers[n][i].isChange = false;
					Render::Get()->_deviceContext->UpdateSubresource((ID3D11Buffer*)_buffers[n][i].buf, 0, NULL, _buffers[n][i].data, 0, 0);
				}
				if(type == SHADER_VS)
					Render::Get()->_deviceContext->VSSetConstantBuffers(_buffers[n][i].index, 1, ((ID3D11Buffer**)&_buffers[n][i].buf));
				else if(type == SHADER_PS)
					Render::Get()->_deviceContext->PSSetConstantBuffers(_buffers[n][i].index, 1, ((ID3D11Buffer**)&_buffers[n][i].buf));
				else if(type == SHADER_GS)
					Render::Get()->_deviceContext->GSSetConstantBuffers(_buffers[n][i].index, 1, ((ID3D11Buffer**)&_buffers[n][i].buf));
				else if(type == SHADER_HS)
					Render::Get()->_deviceContext->HSSetConstantBuffers(_buffers[n][i].index, 1, ((ID3D11Buffer**)&_buffers[n][i].buf));
				else if(type == SHADER_DS)
					Render::Get()->_deviceContext->DSSetConstantBuffers(_buffers[n][i].index, 1, ((ID3D11Buffer**)&_buffers[n][i].buf));
				//break;
			}
		}
	}
	/*void *ConstantBuffer::Get()
	{
		return _buffer;
	}*/
	void ConstantBuffer::Copy(Shader *sh, const string &buffer, const string &field, void *data)
	{
		uint bufId, start, offset;
		if(sh->GetConstant(buffer, field, bufId, start, offset))
			Copy(sh->GetType(), bufId, data, offset, start);
	}
	void ConstantBuffer::Copy(SHADER_TYPE sType, uint bufferId, void *data, UINT size, UINT offset)
	{
		if(bufferId < _buffers[sType].size())
		{
			_buffers[sType][bufferId].isChange = true;
			memcpy(_buffers[sType][bufferId].data + offset, data, size);
		}
	}
	void ConstantBuffer::Clear()
	{
		for(int i = 0; i < 5; ++i)
		{
			for(Buffer &buf : _buffers[i])
			{
				delete[] buf.data;
				buf.buf->Release();
			}
			_buffers[i].clear();
		}
	}
}