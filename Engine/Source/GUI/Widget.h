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
	class Widget: public Style
	{
	protected:
		Pass *z_pass;
		Style *z_style;
		RenderMesh *z_buffer;
		GUI *z_gui;
		Texture *z_texture;
		Widget *z_parent;
		Widget *z_root;
		GUIListener *z_listener;
		vector<Widget*> z_childs;
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
		Widget *GetParent() { return z_parent; };
		GUIListener *GetListener() { return z_listener; };
		RenderMesh *GetVertexBuffer() { return z_buffer; };
		GUI *GetGUI() { return z_gui; };
		Texture *GetTexture() { return z_texture; };
		Property &GetProperty() { return z_prop; };
		string GetName() { return z_name; };
		string GetId() { return z_id; };
		Widget(GUI *gui);
		~Widget();
		Widget *GetByName(const string &name);
		void DeleteChilds();
		bool IsEvent(const string &name);
		bool IsChild(Widget *c, bool depthAll);
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
		Widget *CreateChild(const string &name = "", bool isEvent = false, const string &classes = "");
		GUIListener *CreateListener();
		Widget *GetChild(UINT num);
		UINT GetChildCount();
		void SetTexture(Texture *tex);
		void AddEvent(const string &name);
		bool CurrentEvent(const string &name);
		void RemoveEvent(const string &name);
		void ChangeDisplay();
		void Update(bool isPreview = false);
		void Bake();
		void SetParent(Widget *p);
		void BakeAll();
		virtual void BakeBuffer();
		virtual void Before() {};
		void Rebake(bool andChilds);
		void Draw(bool andChilds = true);
		Widget *GetPick(const Point2 &pos);
	};

	class WidgetNode
	{
	private:
		GUI *_gui;
		Matrix2D _aTransform;
		Matrix2D _rTransform;
		WidgetNode *_parent;
		vector<WidgetNode*> z_childs;
		vector<Vertex2D> v;
		Style _style;
	public:
		WidgetNode *GetParent() { return _parent; };
		Matrix2D &GetAbsoluteTransform() { return _aTransform; };
		Matrix2D &GetRelativeTransform() { return _rTransform; };
		WidgetNode *GetChild(uint num) { if (num < GetChildCount()) return z_childs[num]; return nullptr; };
		uint GetChildCount() { return (uint)z_childs.size(); };

		//WidgetNode(GUI *gui);
		//~WidgetNode();
		//void Bake(const Style &style);//
	};
};