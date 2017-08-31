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
		void SetText(const string& text) {_text = text;}
		string GetText() { return _text; };
		void SetFamily(const string& value) {_family = value;}
		string GetFamily() { return _family; };
		void SetFontSize(uint size) {_fontSize = size;}
		uint GetFontSize() { return _fontSize; };
		void SetTextColor(const Color &color) {_textColor = color;}
		Color GetTextColor() { return _textColor; };
		void SetBold(bool value) {_isBold = value;}
		bool IsBold() { return _isBold; };
		void SetItalic(bool value) {_isItalic = value;}
		bool IsItalic() { return _isItalic; };
	};
};