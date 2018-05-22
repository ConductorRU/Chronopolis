#include "stdafx.h"
#include "WidgetText.h"
#include "../GUI.h"
#include "../Font.h"
#include "../../Math/Rect.h"
#include "../../Render/RenderMesh.h"
#include "../../Render/Direct3d11.h"
#include "../../Material/Pass.h"
#include "../../Material/Texture.h"
namespace DEN
{
	WidgetText::WidgetText(GUI *gui): Widget(gui)
	{
		_buffer->SetTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_isBold = false;
		_isItalic = false;
		_fontSize = 12;
	}
	void WidgetText::_UpdateText()
	{
		size_t size = _text.size();
		char c;
		Rect r;
		float pSize = (float)(int)(_fontSize*0.1);
		float posX = _rect.minX;
		float posY = _rect.minY;
		for(size_t i = 0; i < size; ++i)
		{
			c = _text[i];
			r = _font->GetRect(c);
			posX += float(r.maxX - r.minX) + 1;
		}
		if(!size)
			r = _font->GetRect(' ');
		_rect = Square(_rect.minX, _rect.minY, posX - _rect.minX, float(r.maxY - r.minY));
		/*if(align == "center" || align == "right")
		{
			Square s = _parent->GetSquare();
			float fx = m.minX + parP.minX;
			if(_parent)
			{
				Square sq = _parent->GetSquare();
				if(align == "center")
					fx = float(int(sq.maxX*0.5f - s.maxX*0.5f)) + m.minX - parP.maxX;
				else
					fx = sq.maxX - s.maxX - parP.maxX - m.maxX - m.minX;
				s.minX = s.minX + fx;
				_prop.SetSquare(s);
			}
		}*/
		/*if(z_prop.GetAlign() == 8 || z_prop.GetAlign() == 16)
		{
			s = z_prop.GetSquare();
			float fy = m.minY + parP.minY;
			if(_parent)
			{
				Square sq = _parent->GetProperty().GetSquare();
				if(z_prop.GetAlign() == 8)
					fy = float(int(sq.maxY*0.5f - s.maxY*0.5f)) + m.minY - parP.maxY;
				else
					fy = sq.maxY - s.maxY - parP.maxY - parP.minY - m.maxY - m.minY;
				s.minY = s.minY + fy;
				z_prop.SetSquare(s);
			}
		}*/
		Vertex2D bl;
		memset(&bl, 0, sizeof(Vertex2D));
		v.clear();
		uint n = 0;
		Square s;
		if(_indexes)
			_indexes->clear();
		else
			_indexes = new vector<uint>;
		posX = _offset.minX;
		/*float leftX = posX;
		if(_gui->GetPrevChild())
			posX = _gui->GetPrevChild()->GetOffset().maxX;
		_offset.minX = posX;
		_offset.minY = posY;*/
		float lineHeight = (float)(_font->tm.tmAscent - _font->tm.tmDescent);
		for(size_t i = 0; i < size; ++i)
		{
			c = _text[i];
			s = _font->GetSquare(c);
			r = _font->GetRect(c);
			v.push_back(bl);
			v.push_back(bl);
			v.push_back(bl);
			v.push_back(bl);
			v[n].col = v[n + 1].col = v[n + 2].col = v[n + 3].col = _textColor;
			v[n].uv.x = v[n + 2].uv.x = s.minX;
			v[n + 1].uv.x = v[n + 3].uv.x = s.maxX;
			v[n].uv.y = v[n + 1].uv.y = s.minY;
			v[n + 2].uv.y = v[n + 3].uv.y = s.maxY;

			v[n].pos.x = v[n + 2].pos.x = posX + pSize;
			posX += float(r.maxX - r.minX) + pSize;
			v[n + 1].pos.x = v[n + 3].pos.x = posX;
			v[n].pos.y = v[n + 1].pos.y = posY;
			v[n + 2].pos.y = v[n + 3].pos.y = posY + float(r.maxY - r.minY);
			_indexes->push_back(n);
			_indexes->push_back(n + 1u);
			_indexes->push_back(n + 2u);
			_indexes->push_back(n + 2u);
			_indexes->push_back(n + 1u);
			_indexes->push_back(n + 3u);
			n += 4;
		}
		//_offset.maxX = posX;
		//_offset.maxY = lineHeight;
	}
	void WidgetText::_UpdateFont()
	{
		_fontPass = _gui->GetPass();
		_font = _gui->GetFont();
		if(_family != "")
			_gui->GetFont(_family, _fontSize, _isBold, _isItalic, &_font);
	}
	void WidgetText::_Update()
	{
		_UpdatePosition();
		_UpdateFont();
		_UpdateText();
		_UpdateBackground();
	}
	void WidgetText::_Render(Pass *pass)
	{
		Render *render = Render::Get();
		Texture *tex = pass->GetTexture(0);
		pass->SetTexture(0, _font->GetTexture());
		render->RenderPass(pass);
		render->ExecuteMesh(_buffer);
		pass->SetTexture(0, tex);
	}
};