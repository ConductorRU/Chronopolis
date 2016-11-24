#pragma once
namespace DEN
{
	class Shader;
	typedef enum INPUT_CLASS
	{
		PER_VERTEX = D3D11_INPUT_PER_VERTEX_DATA,
		PER_INSTANCE = D3D11_INPUT_PER_INSTANCE_DATA
	} INPUT_CLASS;
	class InputLayout
	{
	private:
		vector<D3D11_INPUT_ELEMENT_DESC> z_decl;
		ID3D11InputLayout *_il;
		UINT z_size;
	public:
		InputLayout();
		~InputLayout();
		UINT GetCount() { return (UINT)z_decl.size(); };
		UINT GetSize() { return z_size; };
		void *Get() { if(z_decl.size()) return &z_decl[0]; return nullptr; };
		void *GetData() { return _il; };
		int GetOffset(LPCSTR name);
		static UINT GetElementSize(DXGI_FORMAT format);
		void Add(LPCSTR name, UINT index, DXGI_FORMAT format, UINT slot, UINT offset, INPUT_CLASS iClass, UINT stepRate);
		void Add(LPCSTR name, DXGI_FORMAT format);
		void Bake(Shader *vs);
	};
}