#include "stdafx.h"
#include "Mod2Noise.h"
#include "Math/Func.h"
namespace DEN
{
	Mod2Noise::Mod2Noise()
	{
		_isGray = false;
		_isRange = false;
		_density = 1.0f;
		_min = ColorRGB::C_BLACK;
		_max = ColorRGB::C_WHITE;
	}
	Mod2Noise::~Mod2Noise()
	{

	}
	void Mod2Noise::SetRange(bool enable, const ColorRGB &cMin, const ColorRGB &cMax)
	{
		_isRange = enable;
		_min = cMin;
		_max = cMax;
	}
	void Mod2Noise::SetRange(bool enable)
	{
		_isRange = enable;
	}
	void Mod2Noise::SetGrayscale(bool enable)
	{
		_isGray = enable;
	}
	void Mod2Noise::SetDensity(float percent)
	{
		_density = percent;
	}
	void Mod2Noise::Update(RenderTexture *tex)
	{
		uint w = tex->GetTextureDesc().Width;
		uint h = tex->GetTextureDesc().Height;
		ColorRGB col = ColorRGB::C_WHITE;
		tex->Lock(TEXTURE_LOCK_WRITE_DISCARD);
		for(uint x = 0; x < w; ++x)
			for(uint y = 0; y < h; ++y)
			{
				if(_density < 1.0f && _density < Rand(0.0f, 1.0f))
					continue;
				if(_isRange)
					col = ColorRGB(Rand(_min.r, _max.r), Rand(_min.g, _max.g), Rand(_min.b, _max.b));
				else
					col = ColorRGB(255_r, 255_r, 255_r);
				if (_isGray)
					col.b = col.g = col.r;
				tex->SetPixel(x, y, col);
			}
		tex->Unlock();
	}
};