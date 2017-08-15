#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetText : public Widget
	{
	protected:
		string _text;
		void _UpdateText(map<string, string> &inherit);
		void _Update(map<string, string> &inherit);
	public:
		WidgetText(GUI *gui);
		void SetText(const string& text) {_text = text;}
		string GetText() { return _text; };
	};
};