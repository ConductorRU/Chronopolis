#include "stdafx.h"
#include "InputLayout.h"
#include "RenderMesh.h"
#include "Direct3d11.h"
namespace DEN
{
	RenderMesh::RenderMesh(InputLayout *lo): ConstantBuffer()
	{
		_vCount = 0;
		_iCount = 0;
		_vertexBuffer = nullptr;
		_indexBuffer = nullptr;
		_layout = lo;
		_vertexDesc = {0};
		_indexDesc = {0};
		_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	}
	RenderMesh::~RenderMesh()
	{
		if(_vertexBuffer)
			_vertexBuffer->Release();
		if(_indexBuffer)
			_indexBuffer->Release();
	}
	void RenderMesh::Bake(void *vertices, UINT count, UINT elementSize)
	{
		_vCount = count;
		const UINT vertexBufferSize = count*elementSize;
		_vertexDesc.ByteWidth = vertexBufferSize;
		_vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
		_vertexDesc.MiscFlags = 0;
		_vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		_vertexDesc.StructureByteStride = 0;
		HRESULT hr = Render::Get()->_device->CreateBuffer(&_vertexDesc, NULL, &_vertexBuffer);
		void *dat = NULL;
		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT hResult = Render::Get()->_deviceContext->Map((ID3D11Buffer*)_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		dat = resource.pData;
		memcpy(dat, vertices, vertexBufferSize);
		Render::Get()->_deviceContext->Unmap((ID3D11Buffer*)_vertexBuffer, 0);
	}
	void RenderMesh::BakeIndex(void *indices, UINT count)
	{
		_iCount = count;
		const UINT indexBufferSize = count * 4;
		_indexDesc.ByteWidth = indexBufferSize;
		_indexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		_indexDesc.Usage = D3D11_USAGE_DYNAMIC;
		_indexDesc.MiscFlags = 0;
		_indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		_indexDesc.StructureByteStride = 0;
		HRESULT hr = Render::Get()->_device->CreateBuffer(&_indexDesc, NULL, &_indexBuffer);
		void *dat = NULL;
		D3D11_MAPPED_SUBRESOURCE resource;
		HRESULT hResult = Render::Get()->_deviceContext->Map((ID3D11Buffer*)_indexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		dat = resource.pData;
		memcpy(dat, indices, indexBufferSize);
		Render::Get()->_deviceContext->Unmap((ID3D11Buffer*)_indexBuffer, 0);
	}
	void RenderMesh::Update()
	{
		UINT u = 0;
		UINT vSize = _layout->GetSize();
		Render::Get()->_deviceContext->IASetPrimitiveTopology(_topology);
		Render::Get()->_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &vSize, &u);
		if(_iCount)
			Render::Get()->_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		else
			Render::Get()->_deviceContext->IASetIndexBuffer(0, DXGI_FORMAT_R32_UINT, 0);
	}
}