#include "stdafx.h"
#include "Texture.h"
#include "../Render/Direct3d11.h"
namespace DEN
{
	Texture::Texture()
	{
		_state = nullptr;
		_update = true;
		_desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		_desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		_desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		memset(_desc.BorderColor, 0, sizeof(float)*4);
		_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		_desc.MaxAnisotropy = 0;
		_desc.MinLOD = -FLT_MAX;
		_desc.MaxLOD = FLT_MAX;
		_desc.MipLODBias = 0;
	}
	Texture::~Texture()
	{
		if(_state)
			_state->Release();
	}
	void Texture::Render(uint slot)
	{
		if(_update)
		{
			if(_state)
				_state->Release();
			Render::Get()->_device->CreateSamplerState(&_desc, &_state);
		}
		Render::Get()->_deviceContext->PSSetSamplers(slot, 1, &_state);
		Render::Get()->_deviceContext->PSSetShaderResources(slot, 1, (ID3D11ShaderResourceView**)&_res);
	}
}