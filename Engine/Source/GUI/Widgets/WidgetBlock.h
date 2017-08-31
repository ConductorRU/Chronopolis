#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetBlock : public Widget
	{
	protected:
		Square _borderRadius;
		void _UpdateBorders();
		void _Update();
	public:
		WidgetBlock(GUI *gui);
		void SetBorderRadius(const Square &radius);
		Square GetBorderRadius();
	};
};