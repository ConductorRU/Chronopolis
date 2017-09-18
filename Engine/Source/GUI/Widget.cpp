#include "stdafx.h"
#include "../Math/Square.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"
#include "../Math/Rect.h"
#include "../Math/Func.h"
#include "../Common/String.h"
#include "../Material/Pass.h"
#include "../Material/Texture.h"
#include "../Material/Atlas.h"
#include "../Render/RenderMesh.h"
#include "../Render/InputLayout.h"
#include "Font.h"
#include "GUIListener.h"
#include "Widget.h"
#include "GUI.h"
#include "../Render/Direct3d11.h"
#include "../Core/Engine.h"
namespace DEN
{
	WidgetX::WidgetX(GUI *gui)
	{
		z_parent = nullptr;
		z_root = nullptr;
		z_gui = gui;
		z_style = nullptr;
		z_pass = nullptr;
		z_listener = nullptr;
		z_texture = nullptr;
		z_isBake = false;
		z_buffer = new RenderMesh(z_gui->GetInputLayout());
		z_buffer->Init(gui->GetVS());
		z_buffer->Init(gui->GetPS());
		count = 0;
	}
	WidgetX::~WidgetX()
	{
		z_gui->FreeElement(this);
		if(z_parent)
			for(vector<WidgetX*>::const_iterator s = z_parent->z_childs.begin(); s != z_parent->z_childs.end(); ++s)
				if(*s == this)
				{
					z_parent->z_childs.erase(s);
					break;
				}
		DeleteChilds();
		if(z_listener)
		{
			Engine::Get()->GetInput()->RemoveListener(z_listener);
			delete z_listener;
		}
		if(z_root)
			delete z_root;
	}
	WidgetX *WidgetX::GetByName(const string &name)
	{
		for(WidgetX *el : z_childs)
		{
			if(el->GetName() == name)
				return el;
			WidgetX *w = el->GetByName(name);
			if(w)
				return w;
		}
		return nullptr;
	}
	void WidgetX::DeleteChilds()
	{
		while(z_childs.size())
		{
			size_t s = z_childs.size();
			delete z_childs[z_childs.size() - (size_t)1];
			if(s == z_childs.size())
				return;
		}
	}
	bool WidgetX::IsEvent(const string &name)
	{
		if(IsStyleEvent(name))
			return true;
		StyleX *clas;
		for(string &s : z_class)
		{
			clas = z_gui->GetClass(s);
			if(clas && clas->IsStyleEvent(name))
				return true;
		}
		return false;
	}
	bool WidgetX::IsChild(WidgetX *c, bool depthAll)
	{
		for(WidgetX *el : z_childs)
			if(el == c)
				return true;
		if(depthAll)
			for(WidgetX *el : z_childs)
				if(el->IsChild(c, depthAll))
					return true;
		return false;
	}
	void WidgetX::AddClass(const string &name)
	{
		if(!name.size())
			return;
		if(!IsClass(name))
		{
			SetBake(false);
			z_class.push_back(name);
		}
	}
	bool WidgetX::IsClass(const string &name)
	{
		for(string &s : z_class)
			if(s == name)
				return true;
		return false;
	}
	void WidgetX::RemoveClass(const string &name)
	{
		for(vector<string>::const_iterator s = z_class.begin(); s != z_class.end(); ++s)
			if(*s == name)
			{
				SetBake(false);
				z_class.erase(s);
				break;
			}
	}
	void WidgetX::SetData(const string &name, const string &value)
	{
		if(value == "")
		{
			auto it = _data.find(name);
			if(it != _data.end())
				_data.erase(it);
		}
		else
			_data[name] = value;
	}
	string WidgetX::GetData(const string &name)
	{
		auto it = _data.find(name);
		if(it != _data.end())
			return it->second;
		return "";
	}
	bool WidgetX::IsData(const string &name)
	{
		if(_data.find(name) != _data.end())
			return true;
		return false;
	}
	void WidgetX::SetPass(Pass *pass)
	{
		z_pass = pass;
	}
	void WidgetX::CalculateOrder()
	{
		if(z_prop.GetDisplay())
		{
			z_gui->AddOrder(this);
			for(WidgetX *el: z_childs)
				el->CalculateOrder();
			/*UINT cnt = z_childs.size();
			if(cnt)
				for(UINT i = cnt; i > 0; --i)
					z_childs[i - 1U]->CalculateOrder();*/
		}
	}
	void WidgetX::SetName(const string &name)
	{
		z_name = name;
	}
	void WidgetX::SetId(const string &id)
	{
		z_gui->RemoveId(this);
		z_id = id;
		z_gui->AddId(this);
	}
	bool WidgetX::Pick(const Point2 &p, bool andChilds)
	{
		bool pick = z_prop.GetSquare().PickSize(p);
		if(!andChilds || pick)
			return pick;
		for(WidgetX *el : z_childs)
			if(el->GetProperty().GetDisplay())
				if(el->Pick(p, andChilds))
					return true;
		return pick;
	}
	WidgetX *WidgetX::CreateChild(const string &name, bool isEvent, const string &classes)
	{
		WidgetX *ch = z_gui->CreateElement(name, isEvent);
		ch->SetParent(this);
		ch->GetProperty().SetOrder(z_prop.GetOrder() + 1);
		ch->AddClass(classes);
		return ch;
	}
	GUIListener *WidgetX::CreateListener()
	{
		if(z_listener)
			return z_listener;
		//z_listener = new GUIListener(this);
		Engine::Get()->GetInput()->AddListener(z_listener);
		return z_listener;
	}
	WidgetX *WidgetX::GetChild(UINT num)
	{
		if(num < z_childs.size())
			return z_childs[num];
		return nullptr;
	}
	UINT WidgetX::GetChildCount()
	{
		return (uint)z_childs.size();
	}
	void WidgetX::SetTexture(Texture *tex)
	{
		z_texture = tex;
	}
	void WidgetX::SetBake(bool isBake)
	{
		z_isBake = isBake;
		OffUpdate();
		z_prop.OffUpdate();
		if(z_root)
			z_root->SetBake(isBake);
		if(!z_isBake)
			for(WidgetX *e : z_childs)
				e->SetBake(z_isBake);
		++count;
	}
	void WidgetX::AddEvent(const string &name)
	{
		if(z_set.find(name) == z_set.end())
		{
			if(IsStyleEvent(name))
			{
				z_set.insert(name);
				z_update = true;
			}
			else
			{
				StyleX *clas;
				for(string &s : z_class)
				{
					clas = z_gui->GetClass(s);
					if(clas && clas->IsStyleEvent(name))
					{
						z_set.insert(name);
						z_update = true;
					}
				}
			}
		}
	}
	bool WidgetX::CurrentEvent(const string &name)
	{
		if(z_set.find(name) != z_set.end())
			return true;
		return false;
	}

	void WidgetX::RemoveEvent(const string &name)
	{
		if(z_set.find(name) != z_set.end())
		{
			z_set.erase(name);
			if(IsStyleEvent(name))
				z_update = true;
			else
			{
				StyleX *clas;
				for(string &s : z_class)
				{
					clas = z_gui->GetClass(s);
					if(clas && clas->IsStyleEvent(name))
						z_update = true;
				}
			}
		}
	}
	void WidgetX::ChangeDisplay()
	{
		if(z_root)
			delete z_root;
		z_root = nullptr;
		switch(z_prop.GetDisplay())
		{
		case 3://textbox
		case 4://spinner
			z_root = new WidgetX(z_gui);
			z_root->z_parent = this;
			z_root->SetStyle("width:100%; height:100%; color:#000; background-color:#fff; border:1px; padding:2px; border-color:#000");
			WidgetX *text = new WidgetX(z_gui);
			text->SetParent(z_root);
			text->SetStyle("color:inherit; display:text;");
			text->GetProperty().SetInnerText(z_prop.GetInnerText());
			WidgetX *caret = new WidgetX(z_gui);
			caret->SetParent(text);
			caret->SetStyle("display:none; width:1px; height:100%; background-color:#000; word:0");

			GUIListener *lis = z_root->CreateListener();
			lis->onMouseHit = [this, text, caret](MouseEventClick m)
			{
				if(this->Pick(Point2(m.x, m.y)) && m.btn == 0)
				{
					z_gui->SetInputElement(this);
					z_root->SetStyle("border:2px; padding:1px; border-color:#27ae60");
					Square s = text->GetProperty().GetSquare();
					size_t size = text->v.size();
					int num = 0;
					float carX = s.minX;
					if(size > 4)
					{
						for(size_t i = 4; i < size - 3; i += 4)
						{
							float w = text->v[i + 1].pos.x - text->v[i].pos.x;
							if(m.x < text->v[i].pos.x + w*0.5)
							{
								carX = text->v[i].pos.x;
								break;
							}
							else if(i + 5 >= size)
							{
								carX = text->v[i].pos.x + w + 1;
								++num;
								break;
							}
							++num;
						}
					}
					int pos = int(carX - s.minX) - 1;
					caret->SetStyle("display:block; x:" + to_string(pos) + "px; word: " + to_string(num));
				}
				else
				{
					caret->SetStyle("display:none");
					z_root->SetStyle("border:1px; padding:2px; border-color:#000");
					z_gui->SetInputElement(nullptr);
					if(z_listener && z_listener->onChange)
						z_listener->onChange();
				}
				return false;
			};
			lis->onKeyHit = [text, caret, this](KeyEvent k)
			{
				if(caret->Get("display") != "block")
					return false;
				string s;
				size_t num = (size_t)atoi(caret->Get("word").c_str());
				string t = text->GetProperty().GetInnerText();
				if(k.code == KEY_BACK)
				{
					if(num > 0 && t.size() >= num)
					{
						t.erase(num - 1u, 1);
						--num;
					}
				}
				else if(k.code == KEY_RETURN)
				{
					caret->SetStyle("display:none");
					z_root->SetStyle("border:1px; padding:2px; border-color:#000");
					z_gui->SetInputElement(nullptr);
					if(z_listener && z_listener->onChange)
						z_listener->onChange();
				}
				else
					s += Engine::Get()->GetInput()->GetInputText(k.code);
				if(t.size() == 0)
					t = s;
				else
					t.insert(num, s);
				text->GetProperty().SetInnerText(t);
				z_prop.SetInnerText(t);
				num += s.size();
				text->Bake();
				Square sq = text->GetProperty().GetSquare();
				size_t size = text->v.size();
				float carX = sq.minX;
				size_t nNum = 0;
				for(size_t i = 4; i < size - 3; i += 4)
				{
					carX = text->v[i].pos.x;
					++nNum;
					if(nNum > num)
						break;
				}
				if(nNum <= num)
					carX = text->v[size - 3u].pos.x + 1;
				int pos = int(carX - sq.minX) - 1;

				caret->SetStyle("x:" + to_string(pos) + "px; word: " + to_string(num));
				return false;
			};
			lis->onRender = [text, caret, this]()
			{
				if(z_prop.GetInnerText() != text->GetProperty().GetInnerText())
					text->GetProperty().SetInnerText(z_prop.GetInnerText());
				if(caret->Get("display") == "block")
				{
					if((clock() / 500) % 2 == 0)
						caret->GetProperty().SetDisplay(Property::DISPLAY_NONE);
					else
						caret->GetProperty().SetDisplay(Property::DISPLAY_BLOCK);
				}
				return true;
			};
			break;
		}
	}
	void WidgetX::Update(bool isPreview)
	{
		if(!z_enable)
			return;
		Square s, p, m, b, br;
		Vector2 rel;
		if(z_parent)
		{
			s.minX = z_parent->z_prop.GetSquare().minX;
			s.minY = z_parent->z_prop.GetSquare().minY;
			if(z_parent->GetProperty().GetOrder() >= GetProperty().GetOrder())
				GetProperty().SetOrder(z_parent->GetProperty().GetOrder() + 1);
		}
		string name, value;
		map<string, string> fi;
		StyleX *clas;
		for(string &s : z_class)
		{
			clas = z_gui->GetClass(s);
			if(clas)
			{
				auto &fin = clas->z_settings.find("");
				if(fin != clas->z_settings.end())
					for(auto var : fin->second)
						fi[var.first] = var.second;
			}
		}
		auto &fin = z_settings.find("");
		if(fin != z_settings.end())
			for(auto var : fin->second)
				fi[var.first] = var.second;
		if(z_set.size())
		{
			for(string &s : z_class)
			{
				clas = z_gui->GetClass(s);
				if(clas)
				{
					for(string &ord : clas->z_settingOrder)
					{
						auto &it = clas->z_settings.find(ord);
						auto &it2 = z_set.find(ord);
						if(it != clas->z_settings.end() && it2 != z_set.end())
							for(auto var : it->second)
								fi[var.first] = var.second;
					}
				}
			}
		}
		if(z_set.size())
		{
			for(string &ord : z_settingOrder)
			{
				auto &it = z_settings.find(ord);
				auto &it2 = z_set.find(ord);
				if(it != z_settings.end() && it2 != z_set.end())
					for(auto var : it->second)
						fi[var.first] = var.second;
			}
		}
		bool autoX = false;
		bool autoY = false;
		bool autoW = false;
		bool autoH = false;
		int dis = z_prop.GetDisplay();
		for(auto var : fi)
		{
			name = var.first;
			value = var.second;
			if(name == "display")
			{
				if(value == "block")
				{
					z_prop.SetDisplay(Property::DISPLAY_BLOCK);
					continue;
				}
				else if(value == "none")
				{
					z_prop.SetDisplay(Property::DISPLAY_NONE);
					return;
				}
				else if(value == "text")
					z_prop.SetDisplay(Property::DISPLAY_TEXT);
				else if(value == "textbox")
					z_prop.SetDisplay(Property::DISPLAY_TEXTBOX);
				else if(value == "spinner")
					z_prop.SetDisplay(Property::DISPLAY_SPINNER);
			}
			else if(name == "position")
			{
				if(value == "absolute")
					z_prop.SetPosition(1);
				else if(value == "initial")
					z_prop.SetPosition(0);
			}
			else if(name == "align")
			{
				if(value == "left")
					z_prop.SetAlign(0);
				else if(value == "center")
					z_prop.SetAlign(1);
				else if(value == "right")
					z_prop.SetAlign(2);
				else if(value == "top")
					z_prop.SetAlign(4);
				else if(value == "middle")
					z_prop.SetAlign(8);
				else if(value == "bottom")
					z_prop.SetAlign(16);
			}
			else if(name == "white-space")
			{
				if(value == "nowrap")
					z_prop.SetWhiteSpace(1);
				else if(value == "normal")
					z_prop.SetWhiteSpace(0);
			}
			else if(name == "x")
			{
				if(value == "auto")
				{
					autoX = true;
					if(!z_parent)
						s.minX = 0.0;
					else if(z_parent->z_childs.size() == 0 || z_parent->z_childs[0] == this)
						s.minX = z_parent->GetProperty().GetSquare().minX;
					else
					{
						WidgetX *prev = nullptr;
						for(WidgetX *em : z_parent->z_childs)
						{
							if(em == this)
								break;
							prev = em;
						}
						if(prev)
							s.minX = prev->GetProperty().GetSquare().minX + prev->GetProperty().GetSquare().maxX + prev->GetProperty().GetMargin().maxX;
					}
				}
				else
				{
					autoX = false;
					rel.x = GetPixel(name, value, z_parent);
					s.minX += rel.x;
				}
			}
			else if(name == "y")
			{
				if(value == "auto")
				{
					autoY = true;
					if(!z_parent)
						s.minY = 0.0;
					else if(z_parent->z_childs.size() == 0 || z_parent->z_childs[0] == this)
						s.minY = z_parent->GetProperty().GetSquare().minY;
					else
					{
						WidgetX *prev = nullptr;
						for(WidgetX *em : z_parent->z_childs)
						{
							if(em == this)
								break;
							prev = em;
						}
						if(prev)
							s.minY = prev->GetProperty().GetSquare().minY + prev->GetProperty().GetSquare().maxY + prev->GetProperty().GetMargin().maxY;
					}
				}
				else
				{
					autoY = false;
					rel.y = GetPixel(name, value, z_parent);
					s.minY += rel.y;
				}
			}
			else if(name == "padding")
				p.minX = p.minY = p.maxX = p.maxY = GetPixel(name, value, z_parent);
			else if(name == "padding-left")
				p.minX = GetPixel(name, value, z_parent);
			else if(name == "padding-right")
				p.maxX = GetPixel(name, value, z_parent);
			else if(name == "padding-top")
				p.minY = GetPixel(name, value, z_parent);
			else if(name == "padding-bottom")
				p.maxY = GetPixel(name, value, z_parent);
			else if (name == "border-radius")
				br.minX = br.minY = br.maxX = br.maxY = GetPixel(name, value, z_parent);
			else if (name == "border-radius-left")
				br.minX = GetPixel(name, value, z_parent);
			else if (name == "border-radius-right")
				br.maxX = GetPixel(name, value, z_parent);
			else if (name == "border-radius-top")
				br.minY = GetPixel(name, value, z_parent);
			else if (name == "border-radius-bottom")
				br.maxY = GetPixel(name, value, z_parent);
			else if(name == "border")
				b.minX = b.minY = b.maxX = b.maxY = GetPixel(name, value, z_parent);
			else if(name == "border-left")
				b.minX = GetPixel(name, value, z_parent);
			else if(name == "border-right")
				b.maxX = GetPixel(name, value, z_parent);
			else if(name == "border-top")
				b.minY = GetPixel(name, value, z_parent);
			else if(name == "border-bottom")
				b.maxY = GetPixel(name, value, z_parent);
			else if(name == "margin")
				m.minX = m.minY = m.maxX = m.maxY = GetPixel(name, value, z_parent);
			else if(name == "margin-left")
				m.minX = GetPixel(name, value, z_parent);
			else if(name == "margin-right")
				m.maxX = GetPixel(name, value, z_parent);
			else if(name == "margin-top")
				m.minY = GetPixel(name, value, z_parent);
			else if(name == "margin-bottom")
				m.maxY = GetPixel(name, value, z_parent);
			if(name == "width")
			{
				if(value == "auto")
					autoW = true;
				else
				{
					s.maxX = GetPixel(name, value, z_parent);
					autoW = false;
				}
			}
			else if(name == "height")
			{
				if(value == "auto")
					autoH = true;
				else
				{
					s.maxY = GetPixel(name, value, z_parent);
					autoH = false;
				}
			}
			else if(name == "atlas")
			{
				string cl = TwinRemove(trim(value));
				string v1 = param(cl, 1, ' ');
				string v2 = param(cl, 2, ' ');
				if(v1.size() != 0 && v2.size() != 0)
				{
					int texId = atoi(v1.c_str());
					int pieceId = atoi(v2.c_str());
					AtlasImage *img = Engine::Get()->GetAtlas()->GetAtlas(texId);
					SetTexture(img->GetTexture());
					z_prop.SetUV(img->GetUV(pieceId));
					z_prop.SetColor(GetColor(name, value));
				}
			}
			else if(name == "color")
			{
				if(value == "inherit" && z_parent)
					z_prop.SetColor(z_parent->GetProperty().GetColor());
				else
					z_prop.SetColor(GetColor(name, value));
			}
			else if(name == "border-color")
			{
				if(value == "inherit" && z_parent)
					z_prop.SetBorderColor(z_parent->GetProperty().GetBorderColor());
				else
					z_prop.SetBorderColor(GetColor(name, value));
			}
			else if(name == "background-color")
			{
				if(value == "inherit" && z_parent)
					z_prop.SetBackground(z_parent->GetProperty().GetBackground());
				else
					z_prop.SetBackground(GetColor(name, value));
			}
			else if(name == "font-family")
			{
				if(value == "inherit" && z_parent)
					z_prop.SetFont(z_parent->GetProperty().GetFont());
				else
					z_prop.SetFont(value);
			}
			else if(name == "order")
			{
				z_prop.SetOrder(atoi(value.c_str()));
			}
		}
		Square parP;
		if(z_parent && z_prop.GetPosition() != 1)
			parP = z_parent->GetProperty().GetPadding() + z_parent->GetProperty().GetBorder();
		s.minX += m.minX + parP.minX;
		s.minY += m.minY + parP.minY;
		if(autoX && z_parent && z_parent->GetProperty().GetWhiteSpace() == 0)
		{
			float parS = z_parent->z_prop.GetSquare().minX + z_parent->z_prop.GetSquare().maxX;
			if(s.minX + s.maxX > parS)
			{
				float maxY = s.minY;
				for(WidgetX *em : z_parent->z_childs)
				{
					if(em == this)
						break;
					maxY = em->z_prop.GetSquare().minY + em->z_prop.GetSquare().maxY + em->z_prop.GetMargin().maxY;
					//if(maxY != s.minY)
					//	s.minX = em->z_prop.GetSquare().minX + em->z_prop.GetSquare().maxX + em->z_prop.GetMargin().maxX + m.minX;
				}
				s.minX = z_parent->z_prop.GetSquare().minX + m.minX;
				if(maxY == s.minY)
					s.minX += parP.minX;
				s.minY = maxY + m.minY;
			}
			else
			{
				for(WidgetX *em : z_parent->z_childs)
				{
					if(em == this)
						break;
					s.minY = em->z_prop.GetSquare().minY;
				}
			}
		}
		//s.maxX -= m.maxX;
		//s.maxY -= m.maxY;
		z_prop.SetSquare(s);
		z_prop.SetRelative(rel);
		z_prop.SetPadding(p);
		z_prop.SetMargin(m);
		z_prop.SetBorder(b);
		z_prop.SetBorderRadius(br);
		if(dis != z_prop.GetDisplay())
			ChangeDisplay();
		if(autoW)
		{
			s.maxX = 0.0f;
			float minX = 0.0f;
			bool isF = false;
			for(WidgetX *el : z_childs)
			{
				el->Update();
				if(!isF)
					minX = el->GetProperty().GetSquare().minX - el->GetProperty().GetMargin().minX;
				isF = true;
				float size = el->GetProperty().GetSquare().minX + el->GetProperty().GetSquare().maxX + el->GetProperty().GetMargin().maxX - minX;
				if(el->GetProperty().GetPosition() != 1 && s.maxX < size)
					s.maxX = size;
			}
			s.maxX += p.minX + p.maxX;
			z_prop.SetSquare(s);
		}
		if(autoH)
		{
			s.maxY = 0.0f;
			float minY = 0.0f;
			bool isF = false;
			for(WidgetX *el : z_childs)
			{
				el->Update();
				if(!isF)
					minY = el->GetProperty().GetSquare().minY - el->GetProperty().GetMargin().minY;
				isF = true;
				float size = el->GetProperty().GetSquare().minY + el->GetProperty().GetSquare().maxY + el->GetProperty().GetMargin().maxY - minY;
				if(el->GetProperty().GetPosition() != 1 && s.maxY < size)
					s.maxY = size;
			}
			s.maxY += p.minY + p.maxY;
			z_prop.SetSquare(s);
		}
		size_t size = z_prop.GetInnerText().size();
		Font *fontF = z_gui->GetFont();
		if(z_prop.GetFont() != "")
			z_gui->GetFont(z_prop.GetFont(), &fontF);
		if(z_prop.GetDisplay() == 2)
		{
			char c;
			Rect r;
			float posX = s.minX;
			float posY = s.minY;
			for(size_t i = 0; i < size; ++i)
			{
				c = z_prop.GetInnerText()[i];
				r = fontF->GetRect(c);
				posX += float(r.maxX - r.minX) + 1;
			}
			if(!size)
				r = fontF->GetRect(' ');
			z_prop.SetSquare(Square(s.minX, s.minY, posX - s.minX, float(r.maxY - r.minY)));
			if(z_prop.GetAlign() == 1 || z_prop.GetAlign() == 2)
			{
				s = z_prop.GetSquare();
				float fx = m.minX + parP.minX;
				if(z_parent)
				{
					Square sq = z_parent->GetProperty().GetSquare();
					if(z_prop.GetAlign() == 1)
						fx = float(int(sq.maxX*0.5f - s.maxX*0.5f)) + m.minX - parP.maxX;
					else
						fx = sq.maxX - s.maxX - parP.maxX - m.maxX - m.minX;
					s.minX = s.minX + fx;
					z_prop.SetSquare(s);
				}
			}
			if(z_prop.GetAlign() == 8 || z_prop.GetAlign() == 16)
			{
				s = z_prop.GetSquare();
				float fy = m.minY + parP.minY;
				if(z_parent)
				{
					Square sq = z_parent->GetProperty().GetSquare();
					if(z_prop.GetAlign() == 8)
						fy = float(int(sq.maxY*0.5f - s.maxY*0.5f)) + m.minY - parP.maxY;
					else
						fy = sq.maxY - s.maxY - parP.maxY - parP.minY - m.maxY - m.minY;
					s.minY = s.minY + fy;
					z_prop.SetSquare(s);
				}
			}
		}
		else
		{
			if(z_prop.GetAlign() == 1 || z_prop.GetAlign() == 2)
			{
				s = z_prop.GetSquare();
				float fx = 0.0f;
				if(z_parent)
				{
					Square sq = z_parent->GetProperty().GetSquare();
					if(z_prop.GetAlign() == 1)
						fx = float(int(sq.maxX*0.5f - s.maxX*0.5f)) + m.minX - parP.maxX;
					else
						fx = sq.maxX - s.maxX - parP.maxX - parP.minX - m.maxX - m.minX;
					s.minX = s.minX + fx;
					z_prop.SetSquare(s);
				}
			}
			if(z_prop.GetAlign() == 8 || z_prop.GetAlign() == 16)
			{
				s = z_prop.GetSquare();
				float fy = 0.0f;
				if(z_parent)
				{
					Square sq = z_parent->GetProperty().GetSquare();
					if(z_prop.GetAlign() == 8)
						fy = float(int(sq.maxY*0.5f - s.maxY*0.5f)) + m.minY - parP.maxY;
					else
						fy = sq.maxY - s.maxY - parP.maxY - parP.minY - m.maxY - m.minY;
					s.minY = s.minY + fy;
					z_prop.SetSquare(s);
				}
			}
		}
		if (z_listener && z_listener->onUpdate)
			z_listener->onUpdate();
	}
	void WidgetX::Bake()
	{
		if(IsUpdate() || z_prop.IsUpdate())
			SetBake(false);
		if(z_isBake)
		{
			if(z_root)
				z_root->BakeAll();
			return;
		}
		z_gui->BakeCounter();
		Font *fontF = z_gui->GetFont();
		UINT lSize = 0;
		UINT vSize = 0;
		UINT pos = 0;
		Vertex2D bl;
		Square parP;
		memset(&bl, 0, sizeof(Vertex2D));
		bl.col = Color();
		v.clear();
		vector<UINT> in;
		v.push_back(bl);
		v.push_back(bl);
		v.push_back(bl);
		v.push_back(bl);
		in.push_back(0); in.push_back(1); in.push_back(2); in.push_back(2); in.push_back(1); in.push_back(3);
		
		Update();
		Square s = z_prop.GetSquare();
		Square p = z_prop.GetPadding();
		Square m = z_prop.GetMargin();
		Square uv = z_prop.GetUV();
		v[0].uv.x = v[2].uv.x = uv.minX;
		v[0].uv.y = v[1].uv.y = uv.minY;
		v[1].uv.x = v[3].uv.x = uv.maxX;
		v[2].uv.y = v[3].uv.y = uv.maxY;
		
		if(z_prop.GetFont() != "")
			z_gui->GetFont(z_prop.GetFont(), &fontF);

		if(z_parent && z_prop.GetPosition() != 1)
			parP = z_parent->GetProperty().GetPadding();
		v[0].pos.x = v[2].pos.x = z_prop.GetSquare().minX;
		v[0].pos.y = v[1].pos.y = z_prop.GetSquare().minY;
		v[1].pos.x = v[3].pos.x = z_prop.GetSquare().maxX;
		v[2].pos.y = v[3].pos.y = z_prop.GetSquare().maxY;
		v[0].col = v[1].col = v[2].col = v[3].col = z_prop.GetBackground();
		v[1].pos.x += v[0].pos.x;
		v[3].pos.x += v[0].pos.x;
		v[2].pos.y += v[0].pos.y;
		v[3].pos.y += v[0].pos.y;
		size_t size = z_prop.GetInnerText().size();
		if(size > 0 && z_prop.GetDisplay() == 2)
		{
			///z_gui->GetPass()->SetSlot(0, 1);
			uint n = (uint)v.size();
			char c;
			Square s;
			Rect r;
			float posX = float(int(v[0].pos.x));
			float posY = float(int(v[0].pos.y));
			for(size_t i = 0; i < size; ++i)
			{
				c = z_prop.GetInnerText()[i];
				s = fontF->GetSquare(c);
				r = fontF->GetRect(c);
				v.push_back(bl); v.push_back(bl); v.push_back(bl); v.push_back(bl);
				v[n].col = v[n + 1].col = v[n + 2].col = v[n + 3].col = z_prop.GetColor();
				v[n].uv.x = v[n + 2].uv.x = s.minX;
				v[n + 1].uv.x = v[n + 3].uv.x = s.maxX;
				v[n].uv.y = v[n + 1].uv.y = s.minY;
				v[n + 2].uv.y = v[n + 3].uv.y = s.maxY;

				v[n].pos.x = v[n + 2].pos.x = posX + 1;
				posX += float(r.maxX - r.minX) + 1;
				v[n + 1].pos.x = v[n + 3].pos.x = posX;
				v[n].pos.y = v[n + 1].pos.y = posY;
				v[n + 2].pos.y = v[n + 3].pos.y = posY + float(r.maxY - r.minY);
				in.push_back(n); in.push_back(n + 1u); in.push_back(n + 2u); in.push_back(n + 2u); in.push_back(n + 1u); in.push_back(n + 3u);
				n += 4;
			}
			z_buffer->Bake(&v[4], (uint)v.size() - 4u, sizeof(Vertex2D));
			z_buffer->BakeIndex(&in[0], (uint)in.size() - 6u);
		}
		else
		{
			//z_gui->GetPass()->SetSlot(0, 0);
			//z_pass = nullptr;
			z_buffer->Bake(&v[0], (uint)v.size(), sizeof(Vertex2D));
			z_buffer->BakeIndex( &in[0], (uint)in.size());
		}
		OffUpdate();
		z_prop.OffUpdate();
		BakeBuffer();
		if(z_root)
			z_root->BakeAll();
		z_isBake = true;
	}
	void WidgetX::SetParent(WidgetX *p)
	{
		if(z_parent)
			for(vector<WidgetX*>::const_iterator p = z_parent->z_childs.begin(); p != z_parent->z_childs.end(); ++p)
				if(*p == this)
				{
					z_parent->z_childs.erase(p);
					break;
				}
		z_parent = p;
		p->z_childs.push_back(this);
	}
	void WidgetX::Rebake(bool andChilds)
	{
		SetBake(false);
	}
	void WidgetX::BakeAll()
	{
		Bake();
		if(z_prop.GetDisplay())
			for(WidgetX *el : z_childs)
				el->BakeAll();
	}
	void WidgetX::BakeBuffer()
	{
		RenderMesh *buf = GetVertexBuffer();
		buf->UpdateConst();
		float f[2];
		f[0] = 1.0f / (float)Render::Get()->GetWidth();
		f[1] = 1.0f / (float)Render::Get()->GetHeight();
		GetVertexBuffer()->Copy(SHADER_VS, 0, &f, sizeof(f), 0u);
		GetVertexBuffer()->Copy(SHADER_PS, 0, &f, sizeof(f), 0u);
	}
	void WidgetX::Draw(bool andChilds)
	{
		Render *render = Render::Get();
		if(z_prop.GetDisplay())
		{
			if(z_listener && z_listener->onRender)
				z_listener->onRender();
			GetVertexBuffer()->Update();
			GetVertexBuffer()->Unmap();
			if(z_pass)
			{
				render->RenderPass(z_pass);
				render->ExecuteMesh(GetVertexBuffer());
			}
			else
			{
				if(GetTexture())
				{
					Texture *tex = z_gui->GetBlankPass()->GetTexture(0);
					z_gui->GetBlankPass()->SetTexture(0, GetTexture());
					render->RenderPass(z_gui->GetBlankPass());
					render->ExecuteMesh(GetVertexBuffer());
					z_gui->GetBlankPass()->SetTexture(0, tex);
				}
				else
				{
					render->RenderPass(z_gui->GetBlankPass());
					render->ExecuteMesh(GetVertexBuffer());
				}
			}
			if(z_root)
				z_root->Draw(true);
			if(andChilds)
			{
				size_t cnt = z_childs.size();
				if(cnt)
					for(size_t i = cnt; i > 0; --i)
						z_childs[i - (size_t)1]->Draw();
			}
		}
	}
	WidgetX *WidgetX::GetPick(const Point2 &pos)
	{
		if(z_prop.GetDisplay() == 0)
			return nullptr;
		WidgetX *p = nullptr;
		WidgetX *t = nullptr;
		if(Pick(pos, false))
			p = this;
		for(WidgetX *el : z_childs)
			if(el->GetProperty().GetDisplay())
			{
				t = el->GetPick(pos);
				if(t != nullptr && (p == nullptr || p->z_prop.GetOrder() <= t->z_prop.GetOrder()))
					p = t;
			}
		return p;
	}

	Widget::Widget(GUI *gui)
	{
		_gui = gui;
		_parent = nullptr;
		_pass = nullptr;
		_listener = nullptr;
		_buffer = new RenderMesh(_gui->GetInputLayout());
		_buffer->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		_buffer->Init(gui->GetVS());
		_buffer->Init(gui->GetPS());
		_update = true;
		_visible = true;
		_indexes = nullptr;
		_uv = Square(0.0f, 0.0f, 1.0f, 1.0f);
		_align = WIDGET_TOP_LEFT;
	}
	Widget::~Widget()
	{
		if(_indexes)
			delete _indexes;
	}
	GUIListener *Widget::CreateListener()
	{
		if(_listener)
			return _listener;
		_listener = new GUIListener(this);
		Engine::Get()->GetInput()->AddListener(_listener);
		return _listener;
	}
	void Widget::AddTriangle(uint i0, uint i1, uint i2)
	{
		_indexes->push_back(i0);
		_indexes->push_back(i1);
		_indexes->push_back(i2);
	}
	void Widget::AddLine(uint i0, uint i1)
	{
		_indexes->push_back(i0);
		_indexes->push_back(i1);
	}
	void Widget::SetAlign(WIDGET_ALIGN align)
	{
		_align = align;
	}
	WIDGET_ALIGN Widget::GetAlign()
	{
		return _align;
	}
	void Widget::SetWidth(float value, bool isPercent)
	{
		_size.x = value;
	}
	void Widget::SetHeight(float value, bool isPercent)
	{
		_size.y = value;
	}
	void Widget::SetTop(float value, bool isPercent)
	{
		_rect.top = value;
	}
	void Widget::SetLeft(float value, bool isPercent)
	{
		_rect.left = value;
	}
	void Widget::SetRight(float value, bool isPercent)
	{
		_rect.right = value;
	}
	void Widget::SetBottom(float value, bool isPercent)
	{
		_rect.bottom = value;
	}
	void Widget::SetBackgroundColor(const Color &color)
	{
		_background = color;
	}
	Color Widget::GetBackgroundColor()
	{
		return _background;
	}
	Vector2 Widget::GetRealSize()
	{
		return _size - Vector2(_rect.left + _rect.right, _rect.top + _rect.bottom);
	}
	float Widget::PercentWidth(const string &val)
	{
		float p = ((float)atof(val.c_str()))*0.01f;
		if(_parent)
		{
			//Square b = parent->GetProperty().GetBorder();
			//Square pd = parent->GetProperty().GetPadding();
			return (_parent->GetRealSize().x/* - b.maxX - b.minX - pd.maxX - pd.minX*/)*p;
		}
		return float(Render::Get()->GetWidth())*p;
	}
	float Widget::PercentHeight(const string &val)
	{
		float p = ((float)atof(val.c_str()))*0.01f;
		if(_parent)
		{
			//Square b = parent->GetProperty().GetBorder();
			//Square pd = parent->GetProperty().GetPadding();
			return (_parent->GetRealSize().y/* - b.maxY - b.minY - pd.maxY - pd.minY*/)*p;
		}
		return float(Render::Get()->GetHeight())*p;
	}
	float Widget::GetPixel(const string &name, const string &val)
	{
		size_t size = val.size();
		string n;
		string t;
		char op = 0;
		string n1;
		string t1;
		int step = 0;
		for (size_t i = 0; i != size; ++i)
		{
			if (i == 0 && val[i] == '-')
				n += val[i];
			else if (step < 2 && val[i] >= '0' && val[i] <= '9')
			{
				n += val[i];
				step = 1;
			}
			else if (step < 3 && ((val[i] >= 'A' && val[i] <= 'Z') || (val[i] >= 'a' && val[i] <= 'z') || val[i] == '%'))
			{
				step = 2;
				t += val[i];
			}
			else if (step == 2 && (val[i] == '+' || val[i] == '-' || val[i] == '/' || val[i] == '*'))
			{
				step = 3;
				op = val[i];
			}
			else if (step >= 3 && step < 5 && val[i] >= '0' && val[i] <= '9')
			{
				n1 += val[i];
				step = 4;
			}
			else if ((val[i] >= 'A' && val[i] <= 'Z') || (val[i] >= 'a' && val[i] <= 'z') || val[i] == '%')
			{
				step = 5;
				t1 += val[i];
			}
			//else
			//	return 0.0f;
		}
		float v1 = 0.0f;
		float v2 = 0.0f;
		if (t == "px")
			v1 = (float)atof(n.c_str());
		else if (t == "%")
		{
			if (name == "width" || name == "x" || name == "left" || name == "right")
				v1 = PercentWidth(n);
			else if (name == "height" || name == "y" || name == "top" || name == "bottom")
				v1 = PercentHeight(n);
		}
		if (t1 == "px")
		{
			v2 = (float)atof(n1.c_str());
		}
		else if (t1 == "%")
		{
			if (name == "width" || name == "x" || name == "left" || name == "right")
				v2 = PercentWidth(n);
			else if (name == "height" || name == "y" || name == "top" || name == "bottom")
				v2 = PercentHeight(n);
		}
		if (op == '+')
			return v1 + v2;
		if (op == '-')
			return v1 - v2;
		if (op == '/')
			return v1 / v2;
		if (op == '*')
			return v1 * v2;
		return v1;
	}
	Color Widget::GetColor(const string &val)
	{
		Color c;
		if (c.FromHex(val))
			return c;
		return c;
	}
	bool Widget::IsChild(Widget *c, bool depthAll)
	{
		for(Widget *el : z_childs)
			if(el == c)
				return true;
		if(depthAll)
			for(Widget *el : z_childs)
				if(el->IsChild(c, depthAll))
					return true;
		return false;
	}
	bool Widget::Pick(const Point2 &p, bool andChilds)
	{
		bool pick = _offset.Pick(p);
		if(!andChilds || pick)
			return pick;
		for(Widget *el : z_childs)
			//if(el->GetProperty().GetDisplay())
				if(el->Pick(p, andChilds))
					return true;
		return pick;
	}
	void Widget::SetParent(Widget *parent)
	{
		if(_parent)
			for (vector<Widget*>::const_iterator p = _parent->z_childs.begin(); p != _parent->z_childs.end(); ++p)
				if (*p == this)
				{
					_parent->z_childs.erase(p);
					break;
				}
		_parent = parent;
		parent->z_childs.push_back(this);
	}
	void Widget::BakeBuffer()
	{
		_buffer->UpdateConst();
		float f[20];
		f[0] = 1.0f / (float)Render::Get()->GetWidth();
		f[1] = 1.0f / (float)Render::Get()->GetHeight();
		if(_parent)
			_aTransform = _wTransform*_parent->_aTransform*_rTransform;
		else
			_aTransform = _wTransform*_rTransform;
		memcpy(&f[4], &(_aTransform).ToMatrix(), sizeof(float)*4*4);
		_buffer->Copy(SHADER_VS, 0, &f, sizeof(f), 0u);
		_buffer->Copy(SHADER_PS, 0, &f, sizeof(f), 0u);
	}
	void Widget::_UpdatePosition()
	{
		_offset = {0.0f, 0.0f, 0.0f, 0.0f};
		_offset.right = _size.x;
		_offset.bottom = _size.y;
		Square par;
		_offset.top = par.minX;
		_offset.left = par.minY;
		if(_parent)
			par = _parent->GetOffset();
		else
		{
			par.maxX =	(float)Render::Get()->GetWidth();
			par.maxY =	(float)Render::Get()->GetHeight();
		}
		if(_align == WIDGET_TOP_LEFT || _align == WIDGET_MIDDLE_LEFT || _align == WIDGET_BOTTOM_LEFT)
			_offset.minX = par.minX + _rect.minX;
		if(_align == WIDGET_TOP_CENTER || _align == WIDGET_CENTER || _align == WIDGET_BOTTOM_CENTER)
			_offset.minX = par.minX + _rect.minX - _rect.maxX + (par.maxX - _size.x)*0.5;
		if(_align == WIDGET_TOP_RIGHT || _align == WIDGET_MIDDLE_RIGHT || _align == WIDGET_BOTTOM_RIGHT)
			_offset.minX = par.maxX - _rect.maxX - _size.x;
		if(_align == WIDGET_TOP_LEFT || _align == WIDGET_TOP_CENTER || _align == WIDGET_TOP_RIGHT)
			_offset.minY = par.minY + _rect.minY;
		if(_align == WIDGET_MIDDLE_LEFT || _align == WIDGET_CENTER || _align == WIDGET_MIDDLE_RIGHT)
			_offset.minY = par.minY + _rect.minY - _rect.maxY + (par.maxY - _size.y)*0.5;
		if(_align == WIDGET_BOTTOM_LEFT || _align == WIDGET_BOTTOM_CENTER || _align == WIDGET_BOTTOM_RIGHT)
			_offset.minY = par.maxY - _rect.maxY - _size.y;
		_offset.maxX = _offset.minX + _size.x - _rect.left - _rect.right;
		_offset.maxY = _offset.minY + _size.y - _rect.top - _rect.bottom;
		switch(_align)
		{
		case WIDGET_TOP_STRETCH:
			_offset.minX = _rect.minX;
			_offset.minY = _rect.minY;
			_offset.maxX = _offset.minX + par.maxX - par.minX - _rect.maxX;
			_offset.maxY = _offset.minY + _size.y;
			break;
		case WIDGET_STRETCH:
			_offset.minX = _rect.minX;
			_offset.minY = _rect.minY;
			_offset.maxX = _offset.minX + par.maxX - par.minX - _rect.maxX;
			_offset.maxY = _offset.minY + par.maxY - par.minY - _rect.maxY;
			break;
		}
		
	}
	void Widget::_UpdateTransform()
	{
		Vector2 parPos;
		if(_parent)
		{
			parPos.x += _parent->_offset.left;
			parPos.y += _parent->_offset.top;
		}
		_wTransform.SetTranslationX(parPos.x);
		_wTransform.SetTranslationY(parPos.y);
	}
	void Widget::_UpdateBackground()
	{
		
	}
	void Widget::_UpdateAlign()
	{
		vector<Widget*> temp;
		Vector2 align, length;
		bool isUpd = false;
		for(Widget *el : z_childs)
		{
			if(_align == WIDGET_TOP_CENTER || _align == WIDGET_CENTER || _align == WIDGET_BOTTOM_CENTER)
			{
				temp.push_back(el);
				length.x += el->_offset.right - el->_offset.left;
			}
			else if(_align == WIDGET_MIDDLE_LEFT || _align == WIDGET_CENTER || _align == WIDGET_MIDDLE_RIGHT)
			{
				temp.push_back(el);
				length.y += el->_offset.bottom - el->_offset.top;
			}
			if(isUpd)
			{
				isUpd = false;
				//for(Widget *em : temp)
				//	em->_Align(align);
				temp.clear();
				length.x = 0.0f;
				length.y = 0.0f;
			}
		}
		align.x = (_size.x - length.x)*0.5f;
		align.y = (_size.y - length.y)*0.5f;
		//for(Widget *em : temp)
		//	em->_Align(align);
	}
	void Widget::SetStrip(bool isStrip)
	{
		if(isStrip)
			_buffer->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		else
			_buffer->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}
	void Widget::_Update()
	{
		_UpdatePosition();
		_UpdateBackground();
	}
	bool Widget::Update()
	{
		if(!_update)
			return false;
		_update = false;
		_Update();
		return true;
	}
	void Widget::Bake()
	{
		_UpdateTransform();
		if(v.size())
		{
			_buffer->Bake(&v[0], (uint)v.size(), sizeof(Vertex2D));
			if(_indexes && _indexes->size())
				_buffer->BakeIndex(&_indexes->at(0), (uint)_indexes->size());
			BakeBuffer();
		}
	}
	void Widget::BakeAll(map<string, string> inherit)
	{
		if(_gui->GetRoot() == this)
		{
			Update();
			Bake();
		}
		if(z_childs.size())
		{
			Widget *prev = _gui->GetPrevChild();
			_gui->SetPrevChild(nullptr);
			for(Widget *el : z_childs)
			{
				el->Update();
				_gui->SetPrevChild(el);
			}
			_UpdateAlign();
			for(Widget *el : z_childs)
				el->Bake();
			for(Widget *el : z_childs)
				el->BakeAll(inherit);
			_gui->SetPrevChild(prev);
		}
	}
	void Widget::_Render(Pass *pass)
	{
		Render *render = Render::Get();
		render->RenderPass(pass);
		render->ExecuteMesh(_buffer);
	}
	void Widget::Draw(bool andChilds)
	{
		Render *render = Render::Get();
		if (_visible)
		{
			//if (z_listener && z_listener->onRender)
			//	z_listener->onRender();
			_buffer->Update();
			_buffer->Unmap();
			if (_pass)
				_Render(_pass);
			else
				_Render(_gui->GetBlankPass());
			//if(_root)
			//	_root->Draw(true);
			if (andChilds)
			{
				size_t cnt = z_childs.size();
				if (cnt)
					for (size_t i = cnt; i > 0; --i)
						z_childs[i - (size_t)1]->Draw(true);
			}
		}
	}
};