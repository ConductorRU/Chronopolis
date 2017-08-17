#pragma once
#include "../Widget.h"
namespace DEN
{
	class WidgetText : public Widget
	{
	protected:
		string _text;
		Pass *_fontPass;
		class Font *_font;
		void _UpdateFont(map<string, string> &inherit);
		void _UpdateText(map<string, string> &inherit);
		void _Update(map<string, string> &inherit);
		void _Render(Pass *pass);
	public:
		WidgetText(GUI *gui);
		void SetText(const string& text) {_text = text;}
		string GetText() { return _text; };
	};
};