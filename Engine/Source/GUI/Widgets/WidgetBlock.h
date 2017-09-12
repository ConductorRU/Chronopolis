#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetBlock : public Widget
	{
	protected:
		Square _borderRadius;
		Color _borderColor;
		float _borderWidth;
		void _UpdateBorders();
		void _Update();
	public:
		void SetBorderColor(const Color &color) { _borderColor = color;};
		Color GetBorderColor() { return _borderColor;}
		void SetBorderWidth(const float &width) { _borderWidth = width;};
		float GetBorderWidth() { return _borderWidth;}
		WidgetBlock(GUI *gui);
		void SetBorderRadius(const Square &radius);
		Square GetBorderRadius();

	};
};