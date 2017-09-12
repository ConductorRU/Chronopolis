#include "stdafx.h"
#include "WidgetBlock.h"
#include "../GUI.h"
#include "../../Math/Func.h"
#include "../../Math/Vector.h"
namespace DEN
{
	WidgetBlock::WidgetBlock(GUI *gui): Widget(gui)
	{
		_borderWidth = 0.0f;
	}
	void WidgetBlock::SetBorderRadius(const Square &radius)
	{
		_borderRadius = radius;
	}
	Square WidgetBlock::GetBorderRadius()
	{
		return _borderRadius;
	}
	void WidgetBlock::_UpdateBorders()
	{
		string prop;
		Vertex2D t;
		vector<Vertex2D> bord;
		Vertex2D b;
		b.col = _borderColor;
		t.col = _background;
		float val = _borderRadius.minX;
		uint i2 = 0, i3 = 0, i4 = 0;
		if (val > 0)
		{
			if(_indexes)
				_indexes->clear();
			else
				_indexes = new vector<uint>;
			v.clear();
			val = _borderRadius.left_top;
			val = min(val, min(_size.x, _size.y));
			uint id = 0;
			uint i = 0;
			t.pos.x = val + _offset.left;
			t.pos.y = val + _offset.top;
			v.push_back(t);
			for (float f = 0.0f; f <= val; f += 1.0f)//left top
			{
				t.pos.x = val + cos(-PI - (f / val)*PI_HALF)*val + _offset.left;
				t.pos.y = val - sin((f / val)*PI_HALF)*val + _offset.top;
				v.push_back(t);
				if(i > 0)
					AddTriangle(id, id + i, id + i + 1);
				if(_borderWidth > 0.0f)
				{
					b.pos = t.pos;
					bord.push_back(b);
				}
				++i;
			}
			val = _borderRadius.right_top;
			val = min(val, min(_size.x, _size.y));
			t.pos.x = -val + _offset.right;
			t.pos.y = _offset.top;
			v.push_back(t);
			t.pos.x = -val + _offset.right;
			t.pos.y = val + _offset.top;
			v.push_back(t);
			AddTriangle(id, id + i, id + i + 2);
			AddTriangle(id + i, id + i + 2, id + i + 1);
			id += i + 2;
			i2 = id;
			i = 0;
			for (float f = val; f >= 0.0f; f -= 1.0f)//right top
			{
				t.pos.x = -(val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.right;
				t.pos.y = val + sin(PI + (f / val)*PI_HALF)*val + _offset.top;
				v.push_back(t);
				if(i > 0)
					AddTriangle(id, id + i, id + i + 1);
				if(_borderWidth > 0.0f)
				{
					b.pos = t.pos;
					bord.push_back(b);
				}
				++i;
			}
			val = _borderRadius.right_bottom;
			val = min(val, min(_size.x, _size.y));
			t.pos.x = _offset.right;
			t.pos.y = - val + _offset.bottom;
			v.push_back(t);
			t.pos.x = - val + _offset.right;
			t.pos.y = - val + _offset.bottom;
			v.push_back(t);
			AddTriangle(id, id + i, id + i + 2);
			AddTriangle(id + i, id + i + 2, id + i + 1);
			id += i + 2;
			i3 = id;
			i = 0;
			for (float f = 0.0f; f <= val; f += 1.0f)//right bottom
			{
				t.pos.x = - (val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.right;
				t.pos.y = - val + sin((f / val)*PI_HALF)*val + _offset.bottom;
				v.push_back(t);
				if(i > 0)
					AddTriangle(id, id + i, id + i + 1);
				if(_borderWidth > 0.0f)
				{
					b.pos = t.pos;
					bord.push_back(b);
				}
				++i;
			}
			val = _borderRadius.left_bottom;
			val = min(val, min(_size.x, _size.y));
			t.pos.x = val + _offset.left;
			t.pos.y = _offset.bottom;
			v.push_back(t);
			t.pos.x = val + _offset.left;
			t.pos.y = - val + _offset.bottom;
			v.push_back(t);
			AddTriangle(id, id + i, id + i + 2);
			AddTriangle(id + i, id + i + 2, id + i + 1);
			id += i + 2;
			i4 = id;
			i = 0;
			for (float f = val; f >= 0.0f; f -= 1.0f)//left bottom
			{
				t.pos.x = (val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.left;
				t.pos.y = - val - sin(PI + (f / val)*PI_HALF)*val + _offset.bottom;
				v.push_back(t);
				if(i > 0)
					AddTriangle(id, id + i, id + i + 1);
				if(_borderWidth > 0.0f)
				{
					b.pos = t.pos;
					bord.push_back(b);
				}
				++i;
			}
			val = _borderRadius.left_top;
			val = min(val, min(_size.x, _size.y));
			t.pos.x = val + _offset.left;
			t.pos.y = val + _offset.top;
			v.push_back(t);
			t.pos.x = 0.0f + _offset.left;
			t.pos.y = val + _offset.top;
			v.push_back(t);
			AddTriangle(id, id + i, 0);
			AddTriangle(0, 1, id + i);
			AddTriangle(0, i2, i3);
			AddTriangle(i3, i4, 0);
		}
		else
		{
			for (int i = 0; i < 4; ++i)
				v.push_back(t);
			v[0].uv.x = v[2].uv.x = _uv.minX;
			v[2].uv.y = v[3].uv.y = _uv.minY;
			v[1].uv.x = v[3].uv.x = _uv.maxX;
			v[0].uv.y = v[1].uv.y = _uv.maxY;
			v[0].pos.x = v[2].pos.x = _offset.left;
			v[0].pos.y = v[1].pos.y = _offset.top;
			v[1].pos.x = v[3].pos.x = _offset.right;
			v[2].pos.y = v[3].pos.y = _offset.bottom;
			if (v[0].pos.x > v[1].pos.x)
				v[1].pos.x = v[3].pos.x = v[0].pos.x;
			if (v[0].pos.y > v[2].pos.y)
				v[2].pos.y = v[3].pos.y = v[0].pos.y;
		}
		if(_borderWidth > 0.0f)
		{
			Vector2 n, n1, n2;
			uint i0, i2;
			sint count = bord.size();
			for(sint i = 0; i < count; ++i)
			{
				if(i == 0)
					i0 = count - 1u;
				else
					i0 = i - 1u;
				if(i == count - 1u)
					i2 = 0;
				else
					i2 = i + 1;
				n1 = (bord[i0].pos - bord[i].pos);
				n2 = (bord[i2].pos - bord[i].pos);
				n1.Normalize();
				n2.Normalize();
				n = n1 + n2;
				n.Normalize();
				n = -n;
				b.pos = bord[i].pos + n*_borderWidth;
				bord.push_back(b);
			}

			sint vCount = v.size();
			sint bCount = bord.size();
			for(sint i = 0; i < count; ++i)
			{
				v.push_back(bord[i]);
				v.push_back(bord[count + i]);
				if(i != 0)
				{
					AddTriangle(vCount + i*2 - 2, vCount + i*2 - 1, vCount + i*2);
					AddTriangle(vCount + i*2, vCount + i*2 - 1, vCount + i*2 + 1);
				}
				if(i + 1 == count)
				{
					AddTriangle(vCount + i*2, vCount + i*2 - 1, vCount);
					AddTriangle(vCount, vCount + i*2 - 1, vCount + 1);
				}
			}
		}
	}
	void WidgetBlock::_Update()
	{
		_UpdatePosition();
		_UpdateBorders();
		_UpdateBackground();
	}
};