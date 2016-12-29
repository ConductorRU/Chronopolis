#pragma once
#include "../Math/Point2.h"
#include "../Math/Color.h"
#include "Style.h"
namespace DEN
{
	class Pass;
	class RenderMesh;
	class GUI;
	class Texture;
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
		set<string> z_set;
		Property z_prop;
		string z_id;
		string z_name;
		Widget *z_parent;
		Widget *z_root;
		vector<Widget*> z_childs;
		class GUIListener *z_listener;
		bool z_isBake;
		vector<string> z_class;
		vector<Vertex2D> v;
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
		void DeleteChilds();
		bool IsEvent(const string &name);
		bool IsChild(Widget *c, bool depthAll);
		void AddClass(const string &name);
		bool IsClass(const string &name);
		void RemoveClass(const string &name);
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
};