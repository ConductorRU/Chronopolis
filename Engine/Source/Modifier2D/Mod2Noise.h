#pragma once
#include "Modifier2d.h"
#include "Math/ColorRGB.h"
namespace DEN
{
	class Mod2Noise : public Modifier2d
	{
	private:
		bool _isGray;
		bool _isRange;
		float _density;
		ColorRGB _min;
		ColorRGB _max;
	public:
		Mod2Noise();
		~Mod2Noise();
		void SetRange(bool enable, const ColorRGB &cMin, const ColorRGB &cMax);
		void SetRange(bool enable);
		void SetGrayscale(bool enable);
		void SetDensity(float percent);
		void Update(RenderTexture *tex);
	};
};