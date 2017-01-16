#pragma once
#include "../Math/Color.h"
#include "../Math/Vector2.h"
#include "../Math/Square.h"
namespace DEN
{
	class Widget;
	struct Property
	{
	private:
		Square square;
		Square margin;
		Square padding;
		Square border;
		Square borderRadius;
		Square uv;
		Vector2 relative;
		Point2 caret;
		int order;
		int display;
		char position;
		char align;
		char wSpace;
		Color color;
		Color background;
		Color borderColor;
		string innerText;
		string font;
		bool z_isUpdate;
		bool z_isCaret;
	public:
		Property();
		void OffUpdate() { z_isUpdate = false; }
		bool IsUpdate() { return z_isUpdate; };
		bool IsCaret() { return z_isCaret; };
		const Square &GetMargin() const { return margin; }
		const Vector2 &GetRelative() const { return relative; }
		const Square &GetPadding() const { return padding; }
		const Square &GetSquare() const { return square; }
		const Square &GetBorder() const { return border; }
		const Square &GetBorderRadius() const { return borderRadius; }
		const Point2 &GetCaret() const { return caret; }
		const Square &GetUV() const { return uv; }
		const int &GetOrder() const { return order; }
		const int &GetDisplay() const { return display; }
		const char &GetAlign() const { return align; }
		const char &GetPosition() const { return position; }
		const char &GetWhiteSpace() const { return wSpace; }
		const Color &GetColor() const { return color; }
		const Color &GetBorderColor() const { return borderColor; }
		const Color &GetBackground() const { return background; }
		const string &GetInnerText() const { return innerText; }
		const string &GetFont() const { return font; }

		void SetMargin(const Square &var) { if(margin != var) z_isUpdate = true; margin = var; }
		void SetRelative(const Vector2 &var) { if(relative != var) z_isUpdate = true; relative = var; }
		void SetPadding(const Square &var) { if(padding != var) z_isUpdate = true; padding = var; }
		void SetSquare(const Square &var) { if(square != var) z_isUpdate = true; square = var; }
		void SetBorder(const Square &var) { if(border != var) z_isUpdate = true; border = var; }
		void SetBorderRadius(const Square &var) { if (border != var) z_isUpdate = true; borderRadius = var; }
		void SetBorderColor(const Color &var) { if(borderColor != var) z_isUpdate = true; borderColor = var; }
		void SetCaret(const Point2 &var) { if(caret != var) z_isUpdate = true; caret = var; }
		void SetCaret(bool enable) { if(z_isCaret != enable) z_isUpdate = true; z_isCaret = enable; }
		void SetUV(const Square &var) { if(uv != var) z_isUpdate = true; uv = var; }
		void SetAlign(char var) { if(align != var) z_isUpdate = true; align = var; }
		void SetPosition(char var) { if(position != var) z_isUpdate = true; position = var; }
		void SetWhiteSpace(char var) { if(wSpace != var) z_isUpdate = true; wSpace = var; }
		void SetOrder(int var) { if(order != var) z_isUpdate = true;  order = var; }
		void SetDisplay(int var) { if(display != var) z_isUpdate = true;  display = var;  }
		void SetColor(const Color &var) { if(color != var) z_isUpdate = true; color = var; }
		void SetBackground(const Color &var) { if(background != var) z_isUpdate = true; background = var; }
		void SetInnerText(const string &var) { if(innerText != var) z_isUpdate = true; innerText = var; }
		void SetFont(const string &var) { if(font != var) z_isUpdate = true; font = var; }
	};
	class Style
	{
	friend Widget;
	protected:
		bool z_update;
		bool z_enable;
		map<string, map<string, string>> z_settings;
		vector<string> z_settingOrder;
		float PercentWidth(const string &val, Widget *parent = nullptr);
		float PercentHeight(const string &val, Widget *parent = nullptr);
		float GetPixel(const string &name, const string &val, Widget *parent = nullptr);
		Color GetColor(const string &name, const string &val);
		void OffUpdate() { z_update = false; }
	public:
		Style() { z_update = true; z_enable = true; };
		bool IsUpdate() { return z_update; };
		void SetEnable(bool enable);
		void SetValue(const string &name, const string &value, const string &eve = "");
		void SetStyle(const string &style, const string &eve = "");
		void SetEvent(const string &eve, const string &name, const string &value);
		void SetEvent(const string &eve, const string &style);
		string Get(const string &name) const;
		bool IsStyleEvent(const string &name);
	};
};