#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetBlock : public Widget
	{
	protected:
		void _UpdateBorders(map<string, string> &inherit);
		void _Update(map<string, string> &inherit);
	public:
		WidgetBlock(GUI *gui);
	};
};