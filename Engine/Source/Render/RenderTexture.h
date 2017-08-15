#pragma once
#include "../Math/ColorRGB.h"
namespace DEN
{
	enum RESOURCE_TYPE
	{
		RESOURCE_GPU = 0,//можно читать и писать в GPU
		RESOURCE_IMMUTABLE = 1,//можно только читать из GPU
		RESOURCE_DYNAMIC = 2,//можно читать и писать в GPU, и писать в CPU
		RESOURCE_CPU = 3,//можно читать и писать в CPU
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
		D3D11_TEXTURE2D_DESC z_desc;//данные текстурной поверхности
		RESOURCE_TYPE z_type;
		string z_name;//имя файла
		bool z_filter;//фильтрация при загрузке
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
		void FreeTexture();//освободить текстуру
		void Reset();//освободить текстуру (динамические сохраняются в памяти)
		void Set(Color *data);
		void Clear(ColorRGB color);//очистить текстуру текущим цветом (не должна быть заблокирована)
		void SetPixel(UINT x, UINT y, ColorRGB color);//установить пиксель текстуры (требуется блокировка)
		void DrawLine(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color);
		void DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, UINT size = 1);
		void DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, ColorRGB fill);
		void SetPixel(float x, float y, ColorRGB color);//установить пиксель текстуры (требуется блокировка)
		ColorRGB GetPixel(UINT x, UINT y);//получить пиксель текстуры (требуется блокировка)
		ColorRGB GetPixel(float x, float y);//получить пиксель текстуры (требуется блокировка)
		void SetData(UCHAR *data, UINT size = 0); //при size = 0 копируется с размером текстуры (требуется блокировка)
		UCHAR *GetData(UINT &size) const;//возвращает данные текстуры и размер данных (требуется блокировка)
		void ReloadTexture();//загрузить текстуру после Reset
	};
}