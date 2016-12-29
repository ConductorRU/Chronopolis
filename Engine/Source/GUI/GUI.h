#pragma once
#include "../Math/Vector2.h"
#include "../Math/Color.h"

namespace DEN
{
	class Font;
	class Render;
	class VertexShader;
	class PixelShader;
	class InputLayout;
	class Style;
	class Widget;
	class GUI
	{
	private:
		VertexShader *_vs;
		PixelShader *_ps;
		InputLayout *z_input;
		Pass *z_pass;
		Pass *z_passB;
		Font *z_font;
		Texture *z_fontT;
		Texture *z_blank;
		set<Widget*> z_all;
		struct FontPass
		{
			Pass *pass;
			Font *font;
		};
		map<string, FontPass> z_fonts;
		map<string, Style*> z_class;
		map<string, Widget*> z_ids;
		map<int, vector<Widget *>> z_order;
		Widget *z_root;
		Widget *z_picked;
		UINT z_width;
		UINT z_height;
		UINT z_bakeCount;
		Widget *z_inputBox;
	public:
		Pass *GetPass() { return z_pass; };
		VertexShader *GetVS() { return _vs; };
		PixelShader *GetPS() { return _ps; };
		Pass *GetBlankPass() { return z_passB; };
		Texture *GetFontTexture() { return z_fontT; };
		Texture *GetBlank() { return z_blank; };
		InputLayout *GetInputLayout() { return z_input; };
		Widget *GetRoot() { return z_root; };
		Widget *GetInputElement() { return z_inputBox; };
		void BakeCounter() { ++z_bakeCount; };
		UINT GetBakeCounter() { return z_bakeCount; };
		GUI();
		~GUI();
		Widget *GetElementById(const string &name);
		void AddId(Widget *el);
		void RemoveId(Widget *el);
		void FreeElement(Widget *el);
		void SetInputElement(Widget *el);
		bool IsPickChild(Widget *el, bool andEl = true);
		Style *CreateClass(const string &name);
		Style *GetClass(const string &name);
		void AddOrder(Widget *el);
		void ClearOrder();
		Font *Createfont(const string &name, const string &family, int size, bool isBold = false, bool isItalic = false);
		Widget *CreateElement(const string &name = "", bool isEvent = false);
		void Render();
		Font *GetFont();
		void GetFont(const string &name, Font **font, Pass **pass);
	};
};