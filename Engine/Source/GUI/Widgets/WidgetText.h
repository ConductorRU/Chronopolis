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
		uint _fontSize;
		string _family;
		Color _textColor;
		bool _isBold;
		bool _isItalic;
		void _UpdateFont();
		void _UpdateText();
		void _Update();
		void _Render(Pass *pass);
	public:
		WidgetText(GUI *gui);
		void SetText(const string& text) {_text = text; _update = true;}
		string GetText() { return _text; };
		void SetFamily(const string& value) {_family = value; _update = true;}
		string GetFamily() { return _family; };
		void SetFontSize(uint size) {_fontSize = size; _update = true;}
		uint GetFontSize() { return _fontSize; };
		void SetTextColor(const Color &color) {_textColor = color; _update = true;}
		Color GetTextColor() { return _textColor; };
		void SetBold(bool value) {_isBold = value; _update = true;}
		bool IsBold() { return _isBold; };
		void SetItalic(bool value) {_isItalic = value; _update = true;}
		bool IsItalic() { return _isItalic; };
	};
};