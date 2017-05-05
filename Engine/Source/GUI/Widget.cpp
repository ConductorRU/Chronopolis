#include "stdafx.h"
#include "../Math/Square.h"
#include "../Math/Vector2.h"
#include "../Math/Rect.h"
#include "../Math/Func.h"
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
	Widget::Widget(GUI *gui)
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
	Widget::~Widget()
	{
		z_gui->FreeElement(this);
		if(z_parent)
			for(vector<Widget*>::const_iterator s = z_parent->z_childs.begin(); s != z_parent->z_childs.end(); ++s)
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
	Widget *Widget::GetByName(const string &name)
	{
		for(Widget *el : z_childs)
		{
			if(el->GetName() == name)
				return el;
			Widget *w = el->GetByName(name);
			if(w)
				return w;
		}
		return nullptr;
	}
	void Widget::DeleteChilds()
	{
		while(z_childs.size())
		{
			size_t s = z_childs.size();
			delete z_childs[z_childs.size() - (size_t)1];
			if(s == z_childs.size())
				return;
		}
	}
	bool Widget::IsEvent(const string &name)
	{
		if(IsStyleEvent(name))
			return true;
		Style *clas;
		for(string &s : z_class)
		{
			clas = z_gui->GetClass(s);
			if(clas && clas->IsStyleEvent(name))
				return true;
		}
		return false;
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
	void Widget::AddClass(const string &name)
	{
		if(!name.size())
			return;
		if(!IsClass(name))
		{
			SetBake(false);
			z_class.push_back(name);
		}
	}
	bool Widget::IsClass(const string &name)
	{
		for(string &s : z_class)
			if(s == name)
				return true;
		return false;
	}
	void Widget::RemoveClass(const string &name)
	{
		for(vector<string>::const_iterator s = z_class.begin(); s != z_class.end(); ++s)
			if(*s == name)
			{
				SetBake(false);
				z_class.erase(s);
				break;
			}
	}
	void Widget::SetData(const string &name, const string &value)
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
	string Widget::GetData(const string &name)
	{
		auto it = _data.find(name);
		if(it != _data.end())
			return it->second;
		return "";
	}
	bool Widget::IsData(const string &name)
	{
		if(_data.find(name) != _data.end())
			return true;
		return false;
	}
	void Widget::SetPass(Pass *pass)
	{
		z_pass = pass;
	}
	void Widget::CalculateOrder()
	{
		if(z_prop.GetDisplay())
		{
			z_gui->AddOrder(this);
			for(Widget *el: z_childs)
				el->CalculateOrder();
			/*UINT cnt = z_childs.size();
			if(cnt)
				for(UINT i = cnt; i > 0; --i)
					z_childs[i - 1U]->CalculateOrder();*/
		}
	}
	void Widget::SetName(const string &name)
	{
		z_name = name;
	}
	void Widget::SetId(const string &id)
	{
		z_gui->RemoveId(this);
		z_id = id;
		z_gui->AddId(this);
	}
	bool Widget::Pick(const Point2 &p, bool andChilds)
	{
		bool pick = z_prop.GetSquare().PickSize(p);
		if(!andChilds || pick)
			return pick;
		for(Widget *el : z_childs)
			if(el->GetProperty().GetDisplay())
				if(el->Pick(p, andChilds))
					return true;
		return pick;
	}
	Widget *Widget::CreateChild(const string &name, bool isEvent, const string &classes)
	{
		Widget *ch = z_gui->CreateElement(name, isEvent);
		ch->SetParent(this);
		ch->GetProperty().SetOrder(z_prop.GetOrder() + 1);
		ch->AddClass(classes);
		return ch;
	}
	GUIListener *Widget::CreateListener()
	{
		if(z_listener)
			return z_listener;
		z_listener = new GUIListener(this);
		Engine::Get()->GetInput()->AddListener(z_listener);
		return z_listener;
	}
	Widget *Widget::GetChild(UINT num)
	{
		if(num < z_childs.size())
			return z_childs[num];
		return nullptr;
	}
	UINT Widget::GetChildCount()
	{
		return (uint)z_childs.size();
	}
	void Widget::SetTexture(Texture *tex)
	{
		z_texture = tex;
	}
	void Widget::SetBake(bool isBake)
	{
		z_isBake = isBake;
		OffUpdate();
		z_prop.OffUpdate();
		if(z_root)
			z_root->SetBake(isBake);
		if(!z_isBake)
			for(Widget *e : z_childs)
				e->SetBake(z_isBake);
		++count;
	}
	void Widget::AddEvent(const string &name)
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
				Style *clas;
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
	bool Widget::CurrentEvent(const string &name)
	{
		if(z_set.find(name) != z_set.end())
			return true;
		return false;
	}

	void Widget::RemoveEvent(const string &name)
	{
		if(z_set.find(name) != z_set.end())
		{
			z_set.erase(name);
			if(IsStyleEvent(name))
				z_update = true;
			else
			{
				Style *clas;
				for(string &s : z_class)
				{
					clas = z_gui->GetClass(s);
					if(clas && clas->IsStyleEvent(name))
						z_update = true;
				}
			}
		}
	}
	void Widget::ChangeDisplay()
	{
		if(z_root)
			delete z_root;
		z_root = nullptr;
		switch(z_prop.GetDisplay())
		{
		case 3://textbox
		case 4://spinner
			z_root = new Widget(z_gui);
			z_root->z_parent = this;
			z_root->SetStyle("width:100%; height:100%; color:#000; background-color:#fff; border:1px; padding:2px; border-color:#000");
			Widget *text = new Widget(z_gui);
			text->SetParent(z_root);
			text->SetStyle("color:inherit; display:text;");
			text->GetProperty().SetInnerText(z_prop.GetInnerText());
			Widget *caret = new Widget(z_gui);
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
						caret->GetProperty().SetDisplay(0);
					else
						caret->GetProperty().SetDisplay(1);
				}
				return true;
			};
			break;
		}
	}
	void Widget::Update(bool isPreview)
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
		Style *clas;
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
					z_prop.SetDisplay(1);
					continue;
				}
				else if(value == "none")
				{
					z_prop.SetDisplay(0);
					return;
				}
				else if(value == "text")
					z_prop.SetDisplay(2);
				else if(value == "textbox")
					z_prop.SetDisplay(3);
				else if(value == "spinner")
					z_prop.SetDisplay(4);
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
						Widget *prev = nullptr;
						for(Widget *em : z_parent->z_childs)
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
						Widget *prev = nullptr;
						for(Widget *em : z_parent->z_childs)
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
				for(Widget *em : z_parent->z_childs)
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
				for(Widget *em : z_parent->z_childs)
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
			for(Widget *el : z_childs)
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
			for(Widget *el : z_childs)
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
		Pass *fontP = z_gui->GetPass();
		Font *fontF = z_gui->GetFont();
		if(z_prop.GetFont() != "")
			z_gui->GetFont(z_prop.GetFont(), &fontF, &fontP);
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
	void Widget::Bake()
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
		Pass *fontP = z_gui->GetPass();
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
			z_gui->GetFont(z_prop.GetFont(), &fontF, &fontP);

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
			z_pass = fontP;
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
	void Widget::SetParent(Widget *p)
	{
		if(z_parent)
			for(vector<Widget*>::const_iterator p = z_parent->z_childs.begin(); p != z_parent->z_childs.end(); ++p)
				if(*p == this)
				{
					z_parent->z_childs.erase(p);
					break;
				}
		z_parent = p;
		p->z_childs.push_back(this);
	}
	void Widget::Rebake(bool andChilds)
	{
		SetBake(false);
	}
	void Widget::BakeAll()
	{
		Bake();
		if(z_prop.GetDisplay())
			for(Widget *el : z_childs)
				el->BakeAll();
	}
	void Widget::BakeBuffer()
	{
		RenderMesh *buf = GetVertexBuffer();
		buf->UpdateConst();
		float f[20];
		f[0] = 1.0f / (float)Render::Get()->GetWidth();
		f[1] = 1.0f / (float)Render::Get()->GetHeight();
		Square b = z_prop.GetBorder();
		Square s = z_prop.GetSquare();
		memset(&f[4], 0, sizeof(float) * 4);
		if(s.maxX > 0.6f && s.maxY > 0.6f)
		{
			f[4] = b.minX / (s.maxX - 0.5f);
			f[5] = b.minY / (s.maxY - 0.5f);
			f[6] = 1.0f - b.maxX / (s.maxX - 0.5f);
			if(b.maxY > 0.1f)
				f[7] = 1.0f - b.maxY / (s.maxY - 0.5f);
		}
		memcpy(&f[8], &z_prop.GetBorderColor(), sizeof(float) * 4);
		memcpy(&f[12], &z_prop.GetBorderRadius(), sizeof(float) * 4);
		memcpy(&f[16], &z_prop.GetSquare(), sizeof(float) * 4);
		GetVertexBuffer()->Copy(SHADER_VS, 0, &f, sizeof(f), 0u);
		GetVertexBuffer()->Copy(SHADER_PS, 0, &f, sizeof(f), 0u);
	}
	void Widget::Draw(bool andChilds)
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
	Widget *Widget::GetPick(const Point2 &pos)
	{
		if(z_prop.GetDisplay() == 0)
			return nullptr;
		Widget *p = nullptr;
		Widget *t = nullptr;
		if(Pick(pos, false))
			p = this;
		for(Widget *el : z_childs)
			if(el->GetProperty().GetDisplay())
			{
				t = el->GetPick(pos);
				if(t != nullptr && (p == nullptr || p->z_prop.GetOrder() <= t->z_prop.GetOrder()))
					p = t;
			}
		return p;
	}
};