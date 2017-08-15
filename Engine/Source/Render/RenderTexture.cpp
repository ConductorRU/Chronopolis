#include "stdafx.h"
#include "RenderTexture.h"
#include "Direct3d11.h"
#include "../Math/Point2.h"
#include "../Math/Color.h"
#include "../Math/Func.h"
namespace DEN
{
	RenderTexture::RenderTexture()
	{
		z_data = NULL;
		z_dynamic = true;
		z_filter = true;
		ZeroMemory(&z_desc, sizeof(z_desc));
		z_type = RESOURCE_GPU;
		_rowPitch = 0;
	}

	RenderTexture::~RenderTexture()
	{
		FreeTexture();
	}
	void RenderTexture::Lock(TEXTURE_LOCK type, DWORD flags, UINT subID)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		ID3D11DeviceContext *con = Render::Get()->_deviceContext;
		HRESULT hResult = con->Map(_texture, subID, (D3D11_MAP)type, flags, &resource);
		z_data = (UCHAR*)resource.pData;
		_rowPitch = resource.RowPitch;
	}
	void RenderTexture::Unlock(UINT subID)
	{
		Render::Get()->_deviceContext->Unmap(_texture, subID);
		z_data = nullptr;
	}
	void RenderTexture::Create(UINT sizeX, UINT sizeY, RESOURCE_TYPE type, DXGI_FORMAT format, UINT arraySize)
	{
		FreeTexture();
		ID3D11Device *dev = Render::Get()->_device;
		ID3D11DeviceContext *con = Render::Get()->_deviceContext;
		z_type = type;
		z_desc = {0};
		z_desc.Width = sizeX;
		z_desc.Height = sizeY;
		z_desc.ArraySize = arraySize;
		z_desc.Format = format;
		z_desc.SampleDesc.Count = 1;
		z_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if(type == RESOURCE_GPU)
		{
			z_desc.Usage = D3D11_USAGE_DEFAULT;
			z_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			z_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}
		else if(type == RESOURCE_SPRITE)
		{
			z_desc.Usage = D3D11_USAGE_DEFAULT;
			z_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		}
		else if(type == RESOURCE_IMMUTABLE)
		{
			z_desc.Usage = D3D11_USAGE_IMMUTABLE;
		}
		else if(type == RESOURCE_DYNAMIC)
		{
			z_desc.MipLevels = 1;
			z_desc.Usage = D3D11_USAGE_DYNAMIC;
			z_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else if(type == RESOURCE_CPU)
		{
			z_desc.Usage = D3D11_USAGE_STAGING;
			z_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
		}
		else if(type == RESOURCE_RENDER)
		{
			z_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			z_desc.Usage = D3D11_USAGE_DEFAULT;
			z_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			z_desc.MipLevels = 1;
		}
		else if(type == RESOURCE_DEPTH)///заменить
		{
			z_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			z_desc.Usage = D3D11_USAGE_DEFAULT;
			z_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		HRESULT hr;
		if(z_data == NULL)
			hr = dev->CreateTexture2D(&z_desc, NULL, &_texture);//CreateTexture(sizeX, sizeY, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL)
		else
		{
			if(z_desc.Usage != D3D11_USAGE_DYNAMIC || z_desc.Usage != D3D11_USAGE_STAGING)
			{
				hr = dev->CreateTexture2D(&z_desc, NULL, &_texture);
				con->UpdateSubresource(_texture, 0, 0, z_data, sizeX * 4, sizeX*sizeY * 4);
			}
			else
			{
				z_desc.MipLevels = 1;
				z_desc.CPUAccessFlags = 0;
				D3D11_SUBRESOURCE_DATA res;
				res.pSysMem = z_data;
				res.SysMemPitch = sizeX * 4;
				res.SysMemSlicePitch = 0;
				hr = dev->CreateTexture2D(&z_desc, &res, &_texture);
			}
		}
		if(hr != S_OK)
			return;
		if(z_desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));  //zero out shader resouce view description 
			srvDesc.Format = z_desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;  //its an texture array 2d 
			srvDesc.Texture2D.MipLevels = -1;  //we want all slices, so begin at 0
			if(type == RESOURCE_RENDER || type == RESOURCE_SPRITE)
				srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			hr = dev->CreateShaderResourceView(_texture, &srvDesc, &_res);
			if(hr != S_OK)
				return;
			if(type == RESOURCE_GPU)
				con->GenerateMips(_res);
		}
		_texture->GetDesc(&z_desc);
	}
	void RenderTexture::SetRaw(uchar *data, UINT width, UINT height)
	{
		Render::Get()->_deviceContext->UpdateSubresource(_texture, 0, 0, data, width * 4, width*height * 4);
		_texture->GetDesc(&z_desc);
	}
	void RenderTexture::FreeTexture()
	{
		if(_texture)
			_texture->Release();
		if(_res)
			_res->Release();
		_texture = nullptr;
		_res = nullptr;
	}

	void RenderTexture::Reset()
	{
		//if(z_Texture == NULL || z_desc.Pool == D3DPOOL_MANAGED)
		//	return;
		if(z_desc.Usage == D3D11_USAGE_DYNAMIC)
			return;
		this->FreeTexture();
	}

	void RenderTexture::ReloadTexture()
	{
		///if(z_desc.Pool == D3DPOOL_MANAGED)
		///	return;
		if(z_desc.Usage == D3D11_USAGE_DYNAMIC)
			return;
		if(z_dynamic)
		{
			if(z_data != NULL)
			{
				Create( z_desc.Width, z_desc.Height, RESOURCE_DYNAMIC, z_desc.Format);///нужно изменить
			}
			else if(z_desc.Usage == 1)
				Create(z_desc.Width, z_desc.Height, RESOURCE_DYNAMIC, z_desc.Format);///нужно изменить
		}
		//else
		//	GetManager()->GetEngine()->GetMgr()->LoadTexture(this, z_name, z_filter);
	}
	void RenderTexture::Set(Color *data)
	{
		if(!_texture)
			return;
		Lock();
		if(z_data == NULL)
		{
			Unlock();
			return;
		}
		UINT mul, m;
		if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			for(UINT y = 0; y != z_desc.Height; ++y)
				for(UINT x = 0; x != z_desc.Width; ++x)
				{
					m = y*z_desc.Width + x;
					mul = y*_rowPitch + x*4;
					z_data[mul] = min((int)(data[m].b*255.0f), 255);
					z_data[mul + 1] = min((int)(data[m].g*255.0f), 255);
					z_data[mul + 2] = min((int)(data[m].r*255.0f), 255);
					z_data[mul + 3] = min((int)(data[m].a*255.0f), 255);
				}
		}
		Unlock();
	}
	void RenderTexture::Clear(ColorRGB color)
	{
		if(!_texture)
			return;
		Lock(TEXTURE_LOCK_WRITE_DISCARD);
		if(z_data == NULL)
		{
			Unlock();
			return;
		}
		UINT mul;
		if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			for(UINT y = 0; y != z_desc.Height; ++y)
				for(UINT x = 0; x != z_desc.Width; ++x)
				{
					mul = y*_rowPitch + x * 4;
					z_data[mul] = color.b;
					z_data[mul + 1] = color.g;
					z_data[mul + 2] = color.r;
					z_data[mul + 3] = color.a;
				}
		}
		else if(z_desc.Format == DXGI_FORMAT_R32G32_FLOAT)
		{
			Color col = color.ToColor();
			for(UINT y = 0; y != z_desc.Height; ++y)
				for(UINT x = 0; x != z_desc.Width; ++x)
				{
					mul = y*_rowPitch + x * 8;
					memcpy(&z_data[mul], &color.r, 4);
					memcpy(&z_data[mul + 4], &color.g, 4);
				}
		}
		Unlock();
	}

	void RenderTexture::SetPixel(UINT x, UINT y, ColorRGB color)
	{
		if(z_data == NULL || x >= z_desc.Width || y >= z_desc.Height)
			return;
		UINT mul;
		if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			mul = y*_rowPitch + x * 4;
			z_data[mul] = color.b;
			z_data[mul + 1] = color.g;
			z_data[mul + 2] = color.r;
			z_data[mul + 3] = color.a;
		}
	}

	void RenderTexture::DrawLine(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color)
	{
		Point2 p1 = Point2(x1, y1);
		Point2 p2 = Point2(x2, y2);
		Point2 l;
		int a = abs(int(x2) - int(x1));
		int b = abs(int(y2) - int(y1));
		if(a < b)
			a = b;
		if(a == 0)
		{
			SetPixel(UINT(p1.x), UINT(p1.y), color);
			return;
		}
		for(int i = 0; i != a; ++i)
		{
			l.Lerp(p1, p2, float(i) / float(a));
			SetPixel(UINT(l.x), UINT(l.y), color);
		}
	}

	void RenderTexture::DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, UINT size)
	{
		if(size > 1)
		{
			UINT a1, a2, b1, b2;
			a1 = Clamp(int(x1) - size / 2, 0, int(z_desc.Width - 1));
			a2 = Clamp(int(x2) + size / 2, 0, int(z_desc.Width - 1));
			b1 = Clamp(int(y1) - size / 2, 0, int(z_desc.Height - 1));
			b2 = Clamp(int(y2) + size / 2, 0, int(z_desc.Height - 1));
			DrawRect(a1, b1, a2, b2, color, size - 1);
		}
		for(UINT x = x1; x < x2; ++x)
		{
			SetPixel(UINT(x), UINT(y1), color);
			SetPixel(UINT(x), UINT(y2), color);
		}
		for(UINT y = y1; y < y2; ++y)
		{
			SetPixel(UINT(x1), UINT(y), color);
			SetPixel(UINT(x2), UINT(y), color);
		}
	}

	void RenderTexture::DrawRect(UINT x1, UINT y1, UINT x2, UINT y2, ColorRGB color, ColorRGB fill)
	{
		DrawRect(x1, x2, y1, y2, color);
		for(UINT x = x1 + 1; x + 1 < x2; ++x)
			for(UINT y = y1 + 1; y + 1 < y2; ++y)
				SetPixel(UINT(x), UINT(y), fill);
	}

	void RenderTexture::SetPixel(float x, float y, ColorRGB color)
	{
		if(z_data == NULL)
			return;
		UINT tX = (UINT)toInt((float)z_desc.Width*x);
		UINT tY = (UINT)toInt((float)z_desc.Height*y);
		if(tX >= z_desc.Width)
			tX = z_desc.Width - 1;
		if(tY >= z_desc.Height)
			tY = z_desc.Height - 1;
		SetPixel(tX, tY, color);
	}

	ColorRGB RenderTexture::GetPixel(UINT x, UINT y)
	{
		if(z_data == NULL || x >= z_desc.Width || y >= z_desc.Height)
			return ColorRGB::C_BLACK;
		UINT mul;
		ColorRGB color;

		if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			mul = y*_rowPitch + x * 4;
			color.b = z_data[mul];
			color.g = z_data[mul + 1];
			color.r = z_data[mul + 2];
			color.a = z_data[mul + 3];
		}
		return color;
	}

	ColorRGB RenderTexture::GetPixel(float x, float y)
	{
		if(z_data == NULL)
			return ColorRGB::C_BLACK;
		UINT tX = (UINT)toInt((float)z_desc.Width*x);
		UINT tY = (UINT)toInt((float)z_desc.Height*y);
		if(tX >= z_desc.Width)
			tX = z_desc.Width - 1;
		if(tY >= z_desc.Height)
			tY = z_desc.Height - 1;
		return GetPixel(tX, tY);
	}

	void RenderTexture::SetData(UCHAR *data, UINT size)
	{
		if(z_data == NULL)
			return;
		if(size == 0)
		{
			if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
				memcpy(z_data, data, z_desc.Width*z_desc.Height * 4);
		}
		else
			memcpy(z_data, data, size);
	}

	UCHAR *RenderTexture::GetData(UINT &size) const
	{
		size = 0;
		if(z_data == NULL)
			return NULL;
		UCHAR *data = NULL;
		if(z_desc.Format == DXGI_FORMAT_B8G8R8A8_UNORM)
		{
			size = z_desc.Width*z_desc.Height * 4;
			data = new UCHAR[size];
			memcpy(data, z_data, size);
		}
		return data;
	}
}