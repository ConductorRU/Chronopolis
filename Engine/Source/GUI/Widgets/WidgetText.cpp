#include "stdafx.h"
#include "WidgetText.h"
#include "../GUI.h"
#include "../Font.h"
#include "../../Math/Rect.h"
namespace DEN
{
	WidgetText::WidgetText(GUI *gui): Widget(gui)
	{
		
	}
	void WidgetText::_UpdateText(map<string, string> &inherit)
	{
		size_t size = _text.size();
		char c;
		Rect r;
		Font *fontF = _gui->GetFont();
		float posX = _rect.minX;
		float posY = _rect.minY;
		for(size_t i = 0; i < size; ++i)
		{
			c = _text[i];
			r = fontF->GetRect(c);
			posX += float(r.maxX - r.minX) + 1;
		}
		if(!size)
			r = fontF->GetRect(' ');
		_rect = Square(_rect.minX, _rect.minY, posX - _rect.minX, float(r.maxY - r.minY));
		/*if(z_prop.GetAlign() == 1 || z_prop.GetAlign() == 2)
		{
			s = z_prop.GetSquare();
			float fx = m.minX + parP.minX;
			if(_parent)
			{
				Square sq = _parent->_rect;
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
		uint n = (uint)v.size();
		char c;
		Square s;
		Rect r;
		float posX = float(int(v[0].pos.x));
		float posY = float(int(v[0].pos.y));
		for(size_t i = 0; i < size; ++i)
		{
			c = _text[i];
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
	void WidgetText::_Update(map<string, string> &inherit)
	{
		_UpdateTransform(inherit);
		_UpdateText(inherit);
		_UpdateBackground(inherit);
	}
};