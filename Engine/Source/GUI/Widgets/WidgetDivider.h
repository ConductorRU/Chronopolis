#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetDivider: public Widget
	{
	private:
		Widget *_part[2];
		Widget *_line;
	public:
		WidgetDivider(GUI *gui);
		~WidgetDivider();
		Widget *GetPartDivider(bool isSecond = false) { return _part[(int)isSecond]; };
	};
}