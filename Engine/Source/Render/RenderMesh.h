#pragma once
#include "ConstantBuffer.h"
namespace DEN
{
	class InputLayout;
	class RenderMesh: public ConstantBuffer
	{
	private:
		InputLayout *_layout;
		ID3D11Buffer *_vertexBuffer;
		ID3D11Buffer *_indexBuffer;
		D3D11_BUFFER_DESC _vertexDesc;
		D3D11_BUFFER_DESC _indexDesc;
		UINT _vCount;
		UINT _iCount;
	public:
		RenderMesh(InputLayout *lo);
		~RenderMesh();
		UINT GetVertexCount() { return _vCount; };
		UINT GetIndexCount() { return _iCount; };
		InputLayout *GetInputLayout() { return _layout; };
		const D3D11_BUFFER_DESC &GetView() { return _vertexDesc; };
		const D3D11_BUFFER_DESC &GetIndexView() { return _indexDesc; };
		void Bake(void *vertices, UINT count, UINT elementSize);
		void BakeIndex(void *indices, UINT count);
		void Update();
	};
}