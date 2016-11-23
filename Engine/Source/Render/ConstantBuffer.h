#pragma once
namespace DEN
{
	class ConstantBuffer
	{
	private:
		struct ShaderConst
		{
			UINT registerIndex;//register index
			UINT Bytes;
			UINT offset;
			UINT bufferID;
		};
		ID3D11Buffer *_buffer;
		UCHAR *_const;
		UINT _size;
		map<string, ShaderConst> _table;
	public:
		ConstantBuffer()
		{
			_buffer = NULL;
			_const = NULL;
			_size = 0;
		}
		bool Init(UINT size);
		void Map();
		void Unmap();
		void *Get();
		void Copy(void *data, UINT size, UINT offset = 0);
		void Clear();
	};
}