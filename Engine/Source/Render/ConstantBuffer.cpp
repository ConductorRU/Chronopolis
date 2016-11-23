#include "stdafx.h"
#include "ConstantBuffer.h"
#include "Render.h"
namespace DEN
{
	bool ConstantBuffer::Init(UINT size)
	{
		_buffer = NULL;
		_size = size;
		D3D11_BUFFER_DESC desc = {0};
		desc.ByteWidth = size;
		desc.CPUAccessFlags = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.MiscFlags = 0;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.StructureByteStride = 0;
		_const = new UCHAR[size];
		HRESULT hr = Render::Get()->_device->CreateBuffer(&desc, 0, &_buffer);
		if(hr != S_OK)
			return false;
		return true;
	}
	void ConstantBuffer::Map()
	{

	}
	void ConstantBuffer::Unmap()
	{
		Render::Get()->_deviceContext->UpdateSubresource((ID3D11Buffer*)_buffer, 0, NULL, _const, 0, 0);
		Render::Get()->_deviceContext->VSSetConstantBuffers(0, 1, ((ID3D11Buffer**)&_buffer));
	}
	void *ConstantBuffer::Get()
	{
		return _buffer;
	}
	void ConstantBuffer::Copy(void *data, UINT size, UINT offset)
	{
		memcpy(_const + offset, data, size);
	}
	void ConstantBuffer::Clear()
	{
		memset(_const, 0, _size);
	}
}