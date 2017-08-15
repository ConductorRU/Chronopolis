#pragma once
#include "../Math/ColorRGB.h"
namespace DEN
{
	enum RESOURCE_TYPE
	{
		RESOURCE_GPU = 0,//����� ������ � ������ � GPU
		RESOURCE_IMMUTABLE = 1,//����� ������ ������ �� GPU
		RESOURCE_DYNAMIC = 2,//����� ������ � ������ � GPU, � ������ � CPU
		RESOURCE_CPU = 3,//����� ������ � ������ � CPU
		RESOURCE_RENDER = 4,//
		RESOURCE_DEPTH = 5,//
		RESOURCE_SPRITE = 6,//
	};
	enum TEXTURE_LOCK
	{
		TEXTURE_LOCK_READ = 1,
		TEXTURE_LOCK_WRITE = 2,
		TEXTURE_LOCK_READ_WRITE = 3,
		TEXTURE_LOCK_WRITE_DISCARD = 4,
		TEXTURE_LOCK_WRITE_NO_OVERWRITE = 5
	};
	class RenderTexture
	{
		friend struct RenderTarget;
	protected:
		UCHAR* z_data;
		uint _rowPitch;
		bool z_dynamic;
		D3D11_TEXTURE2D_DESC z_desc;//������ ���������� �����������
		RESOURCE_TYPE z_type;
		string z_name;//��� �����
		bool z_filter;//���������� ��� ��������
		ID3D11ShaderResourceView *_res = nullptr;
		ID3D11Texture2D *_texture = nullptr;
	public:
		inline bool IsDynamic() { return z_dynamic; };
		inline RESOURCE_TYPE GetType() { return z_type; };
		inline D3D11_TEXTURE2D_DESC GetTextureDesc() { return z_desc; };
		inline string GetFilename() { return z_name; };
		inline bool IsFilter() { return z_filter; };
		RenderTexture();
		~RenderTexture();
		void Lock(TEXTURE_LOCK type = TEXTURE_LOCK_READ_WRITE, DWORD flags = 0, UINT subID = 0);
		void Unlock(UINT subID = 0);
		void Create(UINT sizeX, UINT sizeY, RESOURCE_TYPE type = RESOURCE_DYNAMIC, DXGI_FORMAT format = DXGI_FORMAT_B8G8R8A8_UNORM, UINT arraySize = 1u);
		void SetRaw(uchar *data, UINT width, UINT height);
		void FreeTexture();//���������� ��������
		void Reset();//���������� �������� (������������ ����������� � ������)
		void Set(Color *data);
		void Clear(ColorRGB color);//�������� �������� ������� ������ (�� ������ ���� �������������)
		void SetPixel(UINT x, UINT y, ColorRGB color);//���������� ������� �������� (��������� ����������)
		void DrawLine(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color);
		void DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, UINT size = 1);
		void DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, ColorRGB fill);
		void SetPixel(float x, float y, ColorRGB color);//���������� ������� �������� (��������� ����������)
		ColorRGB GetPixel(UINT x, UINT y);//�������� ������� �������� (��������� ����������)
		ColorRGB GetPixel(float x, float y);//�������� ������� �������� (��������� ����������)
		void SetData(UCHAR *data, UINT size = 0); //��� size = 0 ���������� � �������� �������� (��������� ����������)
		UCHAR *GetData(UINT &size) const;//���������� ������ �������� � ������ ������ (��������� ����������)
		void ReloadTexture();//��������� �������� ����� Reset
	};
}