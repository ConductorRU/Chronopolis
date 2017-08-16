#pragma once
#include "../Math/Point2.h"
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
	class WidgetX: public Style
	{
	protected:
		Pass *z_pass;
		Style *z_style;
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
		map<string, string> _prop;//CSS => style="" => SetProperty("name", "value");
		string _GetStyle(const string& name, const map<string, string> &inherit);
		bool _update;
		bool _visible;
		void _UpdateTransform(map<string, string> &inherit);
		void _UpdateBackground(map<string, string> &inherit);
		virtual void _Update(map<string, string> &inherit);
		virtual void _Render(Pass *pass);
	public:
		Widget *GetParent() { return _parent; };
		Matrix2D &GetAbsoluteTransform() { return _aTransform; };
		Matrix2D &GetRelativeTransform() { return _rTransform; };
		Widget *GetChild(uint num) { if (num < GetChildCount()) return z_childs[num]; return nullptr; };
		uint GetChildCount() { return (uint)z_childs.size(); };
		Square &GetSquare() { return _rect;}
		Vector2 &GetSize() { return _size;}

		static float PercentWidth(const string &val, Widget *parent = nullptr);
		static float PercentHeight(const string &val, Widget *parent = nullptr);
		static float GetPixel(const string &name, const string &val, Widget *parent = nullptr);
		static Color GetColor(const string &val);
		Widget(GUI *gui);
		~Widget();
		void SetStyle(const string &style, const string &eve = "");
		bool SetProperty(const string& name, const string& value);
		string GetProperty(const string& name);
		void SetParent(Widget *parent);
		void BakeBuffer();
		void Bake(Widget *parent, map<string, string> inherit);//inherit не должно быть по ссылке
		void BakeAll(map<string, string> inherit = {});
		void Draw(bool andChilds = false);
	};

};