#include "stdafx.h"
#include "../Render/InputLayout.h"
#include "../Render/Direct3d11.h"
#include "../Material/Pass.h"
#include "../Material/Shader.h"
#include "../Material/Texture.h"
#include "Font.h"
#include "Widget.h"
#include "GUI.h"
#include "../Input/Input.h"
#include "../Core/Engine.h"
#include "../Manager/Manager.h"
namespace DEN
{
	GUI::GUI()
	{
		z_input = new InputLayout;
		z_input->Add("POSITION", DXGI_FORMAT_R32G32_FLOAT);
		z_input->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
		z_input->Add("UV", DXGI_FORMAT_R32G32_FLOAT);
		z_pass = new Pass;
		_vs = Manager::Get()->CreateVS();
		_ps = Manager::Get()->CreatePS();
		z_pass->SetVS(_vs);
		z_pass->SetPS(_ps);
		_vs->CompileFile(L"gui.hlsl", "mainVS", "4_0");
		_ps->CompileFile(L"gui.hlsl", "mainPS", "4_0");
		z_input->Bake(_vs);
		z_pass->SetDepthEnable(false);
		z_pass->SetDepthFunc(D3DCMPFUNC::D3DCMP_NEVER);
		z_pass->SetDepthWrite(false);
		z_pass->SetAlphaFunc(D3DCMP_GREATEREQUAL);
		z_pass->SetCull(D3DCULL_NONE);
		z_pass->SetAlphaBlend(true);
		z_pass->SetBlend(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
		z_pass->SetAlphaBlend(D3DBLEND_ONE, D3DBLEND_ONE);
		z_font = new Font();
		z_font->LoadFont();
		UINT x, y;
		LPVOID v = z_font->GetRaw(x, y);
		z_fontT = new Texture;
		z_fontT->Create(x, y, RESOURCE_GPU);
		z_fontT->SetRaw((uchar*)v, x, y);
		z_pass->SetTexture(0, z_fontT);

		z_passB = new Pass;
		z_passB->SetVS(_vs);
		z_passB->SetPS(_ps);
		z_passB->SetDepthEnable(false);
		z_passB->SetDepthFunc(D3DCMPFUNC::D3DCMP_NEVER);
		z_passB->SetDepthWrite(false);
		z_passB->SetAlphaFunc(D3DCMP_GREATEREQUAL);
		z_passB->SetCull(D3DCULL_NONE);
		z_passB->SetAlphaBlend(true);
		z_passB->SetBlend(D3DBLEND_SRCALPHA, D3DBLEND_INVSRCALPHA);
		z_passB->SetAlphaBlend(D3DBLEND_ONE, D3DBLEND_ONE);
		z_blank = new Texture;
		z_blank->Create(1, 1);
		z_blank->Lock(TEXTURE_LOCK_WRITE_DISCARD);
		z_blank->SetPixel(0u, 0u, ColorRGB::C_WHITE);
		z_blank->Unlock();
		z_passB->SetTexture(0, z_blank);
		z_root = new WidgetX(this);
		z_root->SetStyle("width: 100%; height:100%; background-color: #0000;");
		_root = new Widget(this);
		_root->SetAlign(WIDGET_STRETCH);
		_root->SetBackgroundColor(Color(0, 0, 0, 0));
		z_width = Render::Get()->GetWidth();
		z_height = Render::Get()->GetHeight();
		z_bakeCount = 0;
		_picked = nullptr;
		z_inputBox = nullptr;
		_prevChild = nullptr;
	}
	GUI::~GUI()
	{
		delete z_root;
		delete z_inputBox;
	}
	map<string, string> GUI::ParseStyles(const string &styles)
	{
		map<string, string> result;
		size_t size = styles.size();
		string name, value;
		bool isVal = false;
		for (size_t i = 0; i <= size; ++i)
		{
			if (i < size && styles[i] != ';')
			{
				if (styles[i] == ':')
					isVal = true;
				else if (!isVal)
				{
					if (styles[i] != ' ' || name.size() > 0)
						name += styles[i];
				}
				else
				{
					if (styles[i] != ' ' || value.size() > 0)
						value += styles[i];
				}
			}
			else
			{
				while(name.size() && name[name.size() - 1U] == ' ')
					name.pop_back();
				while(value.size() && value[value.size() - 1U] == ' ')
					value.pop_back();
				if(name.size() > 0 && value.size() > 0)
					result[name] = value;
				name = "";
				value = "";
				isVal = false;
			}
		}
		return result;
	}
	void GUI::AddSelector(const string& name, const string& value)
	{
		Selector *s = new Selector;
		s->name = name;
		s->props = ParseStyles(value);
		size_t size = name.size();
		string val, typ, hie;
		for(size_t i = 0; i <= size; ++i)
		{
			if(i == size || name[i] == ' ' || name[i] == '.' || name[i] == '#')
			{
				if(val != "")
				{
					SelectorDesc desc;
					desc.name = val;
					if(hie == ">")
						desc.hierarchy = SELECTOR_CHILDS;
					else if(hie == "" && !s->desc.size())
						desc.hierarchy = SELECTOR_ALL;
					else if(hie == "")
						desc.hierarchy = SELECTOR_THIS;
					else if(hie == "+")
						desc.hierarchy = SELECTOR_NEXT;
					else if(hie == "~")
						desc.hierarchy = SELECTOR_NEARBY;
					else if(hie == ":")
						desc.hierarchy = SELECTOR_STATE;

					if(typ == "")
						desc.type = SELECTOR_ANY;
					else if(typ == "#")
						desc.type = SELECTOR_ID;
					else if(typ == ".")
						desc.type = SELECTOR_CLASS;
					else if(typ == ".")
						desc.type = SELECTOR_CLASS;
					else if(typ == ".")
						desc.type = SELECTOR_CLASS;

					s->desc.push_back(desc);
					val.clear();
					typ.clear();
					hie.clear();
				}
				if(i < size && name[i] != ' ')
					typ = name[i];
			}
			else if(i == size || name[i] == '>' || name[i] == '+' || name[i] == '~' || name[i] == ':')
			{
				if(i < size)
					hie = name[i];
			}
			else if(i < size)
				val += name[i];
		}
		_css.push_back(s);
	}
	WidgetX *GUI::GetElementById(const string &name)
	{
		auto &it = z_ids.find(name);
		if(it != z_ids.end())
			return it->second;
		return nullptr;
	}
	void GUI::AddId(WidgetX *el)
	{
		if(el->GetId().size())
			z_ids[el->GetId()] = el;
	}
	void GUI::RemoveId(WidgetX *el)
	{
		if(el->GetId().size())
			z_ids.erase(el->GetId());
	}
	void GUI::FreeElement(WidgetX *el)
	{
		RemoveId(el);
		el = nullptr;
	}
	void GUI::SetInputElement(WidgetX *el)
	{
		z_inputBox = el;
	}
	bool GUI::IsPickChild(Widget *el, bool andEl)
	{
		if(!_picked || (_picked == el && andEl))
			return true;
		return el->IsChild(_picked, true);
	}
	StyleX *GUI::CreateClass(const string &name)
	{
		StyleX *s = new StyleX();
		z_class[name] = s;
		return s;
	}
	StyleX *GUI::GetClass(const string &name)
	{
		auto &it = z_class.find(name);
		if(it != z_class.end())
			return it->second;
		return nullptr;
	}
	void GUI::AddOrder(WidgetX *el)
	{
		z_order[el->GetProperty().GetOrder()].push_back(el);
	}
	void GUI::ClearOrder()
	{
		z_order.clear();
	}
	Font *GUI::Createfont(const string &name, const string &family, int size, bool isBold, bool isItalic)
	{
		Font *font = new Font();
		font->LoadFont(family, size, isBold, isItalic);
		if(name != "")
			z_fonts[name] = font;
		string autoName = family + "_" + to_string(size) + (isBold ? "b" : "") + (isItalic ? "i" : "");
		z_autofonts[autoName] = font;
		return font;
	}
	WidgetX *GUI::CreateElement(const string &name, bool isEvent)
	{
		WidgetX *el = new WidgetX(this);
		el->SetName(name);
		if(isEvent)
			el->CreateListener();
		el->SetParent(z_root);
		z_all.insert(el);
		return el;
	}
	void GUI::Render()
	{
		z_order.clear();
		if(Engine::Get()->GetTime().fNum == 1)
			z_bakeCount = 0;
		if(z_width != Render::Get()->GetWidth() || z_height != Render::Get()->GetHeight())
		{
			z_width = Render::Get()->GetWidth();
			z_height = Render::Get()->GetHeight();
			//z_root->Rebake(true);
		}
		/*z_root->BakeAll();
		z_root->CalculateOrder();
		Point2 cur = Engine::Get()->GetInput()->GetCursorPos();
		z_picked = z_root->GetPick(cur);
		auto &it = z_order.begin();
		for(auto &it : z_order)
			for(auto &it2 : it.second)
			{
				it2->Before();
				it2->Draw(false);
			}*/
		_root->BakeAll();
		_root->Draw(true);

	}
	Font *GUI::GetFont()
	{
		return z_font;
	}
	void GUI::GetFont(const string &name, Font **font)
	{
		auto f = z_fonts.find(name);
		if(f != z_fonts.end())
			*font = f->second;
		else
			*font = z_font;
	}
	void GUI::GetFont(const string &family, int size, bool isBold, bool isItalic, Font **font)
	{
		string autoName = family + "_" + to_string(size) + (isBold ? "b" : "") + (isItalic ? "i" : "");
		auto f = z_autofonts.find(family);
		if(f != z_autofonts.end())
			*font = f->second;
		else
		{
			Font *fn = Createfont("", family, size, isBold, isItalic);
			if(fn)
				*font = fn;
			else
				*font = z_font;
		}
	}
};