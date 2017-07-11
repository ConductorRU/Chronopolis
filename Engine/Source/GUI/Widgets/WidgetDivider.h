#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetDivider: public WidgetX
	{
	private:
		WidgetX *_part[2];
		WidgetX *_line;
	public:
		WidgetDivider(GUI *gui);
		~WidgetDivider();
		WidgetX *GetPartDivider(bool isSecond = false) { return _part[(int)isSecond]; };
	};
}