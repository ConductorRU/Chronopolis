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
	class StyleX;
	class WidgetX;
	class Widget;
	class Pass;
	class Texture;
	enum SELECTOR_TYPE
	{
		SELECTOR_ANY       = 0,
		SELECTOR_ID        = 1,
		SELECTOR_CLASS     = 2,
		SELECTOR_ATTRIBUTE = 3,
		SELECTOR_WIDGET    = 4,
	};
	enum SELECTOR_HIERARCHY
	{
		SELECTOR_ALL       = 0,
		SELECTOR_THIS      = 1,
		SELECTOR_CHILDS    = 2,
		SELECTOR_NEXT      = 3,
		SELECTOR_NEARBY    = 4,
		SELECTOR_STATE     = 5,
	};
	struct SelectorDesc
	{
		string name;
		SELECTOR_TYPE type;//class, id or...
		SELECTOR_HIERARCHY hierarchy;//this
	};
	struct Selector
	{
		string name;
		vector<SelectorDesc> desc;
		map<string, string> props;
	};
	class GUI
	{
	friend Widget;
	private:
		VertexShader *_vs;
		PixelShader *_ps;
		InputLayout *z_input;
		Pass *z_pass;
		Pass *z_passB;
		Font *z_font;
		Texture *z_fontT;
		Texture *z_blank;
		set<WidgetX*> z_all;
		map<string, Font*> z_fonts;
		map<string, Font*> z_autofonts;
		map<string, StyleX*> z_class;
		map<string, Widget*> _ids;
		vector<Selector*> _css;
		map<int, vector<WidgetX *>> z_order;
		WidgetX *z_root;
		Widget *_root;
		Widget *_prevChild;
		Widget *_picked;
		UINT z_width;
		UINT z_height;
		UINT z_bakeCount;
		WidgetX *z_inputBox;

	public:
		Pass *GetPass() { return z_pass; };
		VertexShader *GetVS() { return _vs; };
		PixelShader *GetPS() { return _ps; };
		Pass *GetBlankPass() { return z_passB; };
		Texture *GetFontTexture() { return z_fontT; };
		Texture *GetBlank() { return z_blank; };
		InputLayout *GetInputLayout() { return z_input; };
		Widget *GetRoot() { return _root; };
		WidgetX *GetInputElement() { return z_inputBox; };
		void BakeCounter() { ++z_bakeCount; };
		UINT GetBakeCounter() { return z_bakeCount; };
		Widget *GetPrevChild() { return _prevChild; };
		void SetPrevChild(Widget *widget) { _prevChild = widget; };
		GUI();
		~GUI();
		static map<string, string> ParseStyles(const string &styles);
		void AddSelector(const string& name, const string& value);
		Widget *GetElementById(const string &name);
		void FreeElement(WidgetX *el);
		void SetInputElement(WidgetX *el);
		bool IsPickChild(Widget *el, bool andEl = true);
		StyleX *CreateClass(const string &name);
		StyleX *GetClass(const string &name);
		void AddOrder(WidgetX *el);
		void ClearOrder();
		Font *Createfont(const string &name, const string &family, int size, bool isBold = false, bool isItalic = false);
		WidgetX *CreateElement(const string &name = "", bool isEvent = false);
		void Render();
		Font *GetFont();
		void GetFont(const string &name, Font **font);
		void GetFont(const string &family, int size, bool isBold, bool isItalic, Font **font);


	};
};