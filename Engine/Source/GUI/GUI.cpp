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
		z_pass->SetAlphaBlend(D3DBLEND_ONE, D3DBLEND_ZERO);
		z_font = new Font();
		z_font->LoadFont();
		UINT x, y;
		LPVOID v = z_font->GetRaw(x, y);
		z_fontT = new Texture;
		z_fontT->Create(x, y, RESOURCE_GPU);
		z_fontT->SetRaw((char*)v, x, y);
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
		z_passB->SetAlphaBlend(D3DBLEND_ONE, D3DBLEND_ZERO);
		z_blank = new Texture;
		z_blank->Create(1, 1);
		z_blank->Lock(TEXTURE_LOCK_WRITE_DISCARD);
		z_blank->SetPixel(0u, 0u, ColorRGB::C_WHITE);
		z_blank->Unlock();
		z_passB->SetTexture(0, z_blank);
		z_root = new Widget(this);
		z_root->SetStyle("width: 100%; height:100%; background-color: #0000;");
		z_width = Render::Get()->GetWidth();
		z_height = Render::Get()->GetHeight();
		z_bakeCount = 0;
		z_picked = nullptr;
		z_inputBox = nullptr;
	}
	GUI::~GUI()
	{
		delete z_root;
		delete z_inputBox;
	}
	Widget *GUI::GetElementById(const string &name)
	{
		auto &it = z_ids.find(name);
		if(it != z_ids.end())
			return it->second;
		return nullptr;
	}
	void GUI::AddId(Widget *el)
	{
		if(el->GetId().size())
			z_ids[el->GetId()] = el;
	}
	void GUI::RemoveId(Widget *el)
	{
		if(el->GetId().size())
			z_ids.erase(el->GetId());
	}
	void GUI::FreeElement(Widget *el)
	{
		RemoveId(el);
		el = nullptr;
	}
	void GUI::SetInputElement(Widget *el)
	{
		z_inputBox = el;
	}
	bool GUI::IsPickChild(Widget *el, bool andEl)
	{
		if(!z_picked || (z_picked == el && andEl))
			return true;
		return el->IsChild(z_picked, true);
	}
	Style *GUI::CreateClass(const string &name)
	{
		Style *s = new Style();
		z_class[name] = s;
		return s;
	}
	Style *GUI::GetClass(const string &name)
	{
		auto &it = z_class.find(name);
		if(it != z_class.end())
			return it->second;
		return nullptr;
	}
	void GUI::AddOrder(Widget *el)
	{
		z_order[el->GetProperty().GetOrder()].push_back(el);
	}
	void GUI::ClearOrder()
	{
		z_order.clear();
	}
	Font *GUI::Createfont(const string &name, const string &family, int size, bool isBold, bool isItalic)
	{
		Pass *pass = new Pass;
		pass->SetVS(_vs);
		pass->SetPS(_ps);
		Font *font = new Font();
		font->LoadFont(family, size, isBold, isItalic);
		UINT x, y;
		LPVOID v = font->GetRaw(x, y);
		Texture *fontT = new Texture;
		fontT->Create(x, y);
		fontT->SetRaw((char*)v, x, y);
		pass->SetTexture(0, fontT);
		z_fonts[name] = {pass, font};
		return font;
	}
	Widget *GUI::CreateElement(const string &name, bool isEvent)
	{
		Widget *el = new Widget(this);
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
			z_root->Rebake(true);
		}
		z_root->BakeAll();
		z_root->CalculateOrder();
		Point2 cur = Engine::Get()->GetInput()->GetCursorPos();
		z_picked = z_root->GetPick(cur);
		auto &it = z_order.begin();
		for(auto &it : z_order)
			for(auto &it2 : it.second)
			{
				it2->Before();
				it2->Draw(false);
			}
		//z_root->Render(render);
	}
	Font *GUI::GetFont()
	{
		return z_font;
	}
	void GUI::GetFont(const string &name, Font **font, Pass **pass)
	{
		auto f = z_fonts.find(name);
		if(f != z_fonts.end())
		{
			*pass = f->second.pass;
			*font = f->second.font;
		}
		else
		{
			*pass = z_pass;
			*font = z_font;
		}
	}
};