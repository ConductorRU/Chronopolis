#pragma once
#include "../Math/Point2.h"
#include "../Math/Square.h"
#include "../Math/Color.h"
#include "../Math/Matrix2D.h"
#include "Style.h"
namespace DEN
{
	class Pass;
	class RenderMesh;
	class GUI;
	class Texture;
	class GUIListener;
	struct Vertex2D
	{
		Vector2 pos;
		Color col;
		Vector2 uv;
	};
	class WidgetX: public StyleX
	{
	protected:
		Pass *z_pass;
		StyleX *z_style;
		RenderMesh *z_buffer;
		GUI *z_gui;
		Texture *z_texture;
		WidgetX *z_parent;
		WidgetX *z_root;
		GUIListener *z_listener;
		vector<WidgetX*> z_childs;
		vector<string> z_class;
		vector<Vertex2D> v;
		set<string> z_set;
		map<string, string> _data;
		Property z_prop;
		string z_id;
		string z_name;
		bool z_isBake;
		void SetBake(bool isBake);
	public:
		int count;
		WidgetX *GetParent() { return z_parent; };
		GUIListener *GetListener() { return z_listener; };
		RenderMesh *GetVertexBuffer() { return z_buffer; };
		GUI *GetGUI() { return z_gui; };
		Texture *GetTexture() { return z_texture; };
		Property &GetProperty() { return z_prop; };
		string GetName() { return z_name; };
		string GetId() { return z_id; };
		WidgetX(GUI *gui);
		~WidgetX();
		WidgetX *GetByName(const string &name);
		void DeleteChilds();
		bool IsEvent(const string &name);
		bool IsChild(WidgetX *c, bool depthAll);
		void AddClass(const string &name);
		bool IsClass(const string &name);
		void RemoveClass(const string &name);
		void SetData(const string &name, const string &value);
		string GetData(const string &name);
		bool IsData(const string &name);
		void SetPass(Pass *pass);
		void CalculateOrder();
		void SetName(const string &name);
		void SetId(const string &id);
		bool Pick(const Point2 &p, bool andChilds = false);
		WidgetX *CreateChild(const string &name = "", bool isEvent = false, const string &classes = "");
		GUIListener *CreateListener();
		WidgetX *GetChild(UINT num);
		UINT GetChildCount();
		void SetTexture(Texture *tex);
		void AddEvent(const string &name);
		bool CurrentEvent(const string &name);
		void RemoveEvent(const string &name);
		void ChangeDisplay();
		void Update(bool isPreview = false);
		void Bake();
		void SetParent(WidgetX *p);
		void BakeAll();
		virtual void BakeBuffer();
		virtual void Before() {};
		void Rebake(bool andChilds);
		void Draw(bool andChilds = true);
		WidgetX *GetPick(const Point2 &pos);
	};

	struct WidgetProp
	{
		string name;
		string value;
	};
	enum WIDGET_ALIGN
	{
		WIDGET_TOP_LEFT       = 0,
		WIDGET_TOP_CENTER     = 1,
		WIDGET_TOP_RIGHT      = 2,
		WIDGET_TOP_STRETCH    = 3,
		WIDGET_MIDDLE_LEFT    = 4,
		WIDGET_CENTER         = 5,
		WIDGET_MIDDLE_RIGHT   = 6,
		WIDGET_MIDDLE_STRETCH = 7,
		WIDGET_BOTTOM_LEFT    = 8,
		WIDGET_BOTTOM_CENTER  = 9,
		WIDGET_BOTTOM_RIGHT   = 10,
		WIDGET_BOTTOM_STRETCH = 11,
		WIDGET_STRETCH_LEFT   = 12,
		WIDGET_STRETCH_TOP    = 13,
		WIDGET_STRETCH_BOTTOM = 14,
		WIDGET_STRETCH        = 15,
		WIDGET_INLINE         = 16,
	};
	class Widget
	{
	protected:
		GUI *_gui;
		Widget *_parent;
		RenderMesh *_buffer;
		Pass *_pass;
		vector<Widget*> z_childs;
		vector<Vertex2D> v;
		vector<uint> *_indexes;
		Matrix2D _aTransform;
		Matrix2D _rTransform;
		Square _rect;//top, left, right, bottom
		Vector2 _size;//width, height
		WIDGET_ALIGN _align;
		Square _offset;
		Square _uv;
		Color _background;
		bool _update;
		bool _visible;
		void _UpdatePosition();
		void _UpdateTransform();
		void _UpdateBackground();
		void _UpdateAlign();
		virtual void _Update();
		virtual void _Render(Pass *pass);
	public:
		Widget *GetParent() { return _parent; };
		Matrix2D &GetAbsoluteTransform() { return _aTransform; };
		Matrix2D &GetRelativeTransform() { return _rTransform; };
		GUI *GetGUI() { return _gui; };
		Square &GetOffset() { return _offset; };
		Widget *GetChild(uint num) { if (num < GetChildCount()) return z_childs[num]; return nullptr; };
		uint GetChildCount() { return (uint)z_childs.size(); };
		Square &GetSquare() { return _rect;}
		Vector2 &GetSize() { return _size;}

		float PercentWidth(const string &val);
		float PercentHeight(const string &val);
		float GetPixel(const string &name, const string &val);
		static Color GetColor(const string &val);
		Widget(GUI *gui);
		~Widget();
		void AddTriangle(uint i0, uint i1, uint i2);
		void SetAlign(WIDGET_ALIGN align);
		WIDGET_ALIGN GetAlign();
		void SetWidth(float value, bool isPercent = false);
		void SetHeight(float value, bool isPercent = false);
		void SetTop(float value, bool isPercent = false);
		void SetLeft(float value, bool isPercent = false);
		void SetRight(float value, bool isPercent = false);
		void SetBottom(float value, bool isPercent = false);

		void SetBackgroundColor(const Color &color);
		Color GetBackgroundColor();
		bool IsChild(Widget *c, bool depthAll);
		bool Pick(const Point2 &p, bool andChilds = false);
		void SetParent(Widget *parent);
		void BakeBuffer();
		bool Update();
		void Bake();
		void BakeAll(map<string, string> inherit = {});
		void Draw(bool andChilds = false);
	};

};