#include "stdafx.h"
#include "WidgetBlock.h"
#include "../GUI.h"
#include "../../Math/Func.h"
namespace DEN
{
	WidgetBlock::WidgetBlock(GUI *gui): Widget(gui)
	{
		_prop["display"] = "block";
	}
	void WidgetBlock::_UpdateBorders(map<string, string> &inherit)
	{
		string prop;
		Vertex2D t;
		float val;
		memset(&t, 0, sizeof(Vertex2D));

		prop = GetStyle("position", inherit);
		Square siz = {0.0f, 0.0f, 0.0f, 0.0f};
		siz.right = _size.x;
		siz.bottom = _size.y;
		if(prop == "relative")
		{
			siz = _rect;
			siz.right = _size.x - _rect.right;
			siz.bottom = _size.y - _rect.bottom;
		}
		else if(prop == "absolute")
		{
			if(_rectEnable[0] && _rectEnable[2])
			{
				siz.top = _rect.top;
				siz.bottom = _parent->GetSquare().maxY - _rect.bottom;
			}
			else if(_rectEnable[0])
			{
				siz.top = _rect.top;
				siz.bottom = _rect.top + _size.y;
			}
			else if(_rectEnable[2])
			{
				siz.top = _parent->GetSize().y - _size.y - _rect.top;
				siz.bottom = siz.top + _size.y;
			}

			if(_rectEnable[1] && _rectEnable[3])
			{
				siz.left = _rect.left;
				siz.right = _parent->GetSquare().maxX - _rect.right;
			}
			else if(_rectEnable[1])
			{
				siz.left = _rect.left;
				siz.right = _rect.left + _size.x;
			}
			else if(_rectEnable[3])
			{
				siz.left = _parent->GetSize().x - _size.x - _rect.left;
				siz.right = siz.left + _size.x;
			}
		}
		prop = GetStyle("border-radius", inherit);
		if (prop != "")
		{
			val = GetPixel("border-radius", prop);
			if (val > 0)
			{
				v.clear();
				val = min(val, min(_size.x, _size.y));
				for (float f = 0.0f; f <= val; f += 1.0f)//left top
				{
					t.pos.x = val + siz.left;
					t.pos.y = val + siz.top;
					v.push_back(t);
					t.pos.x = val + cos(-PI - (f / val)*PI_HALF)*val + siz.left;
					t.pos.y = val - sin((f / val)*PI_HALF)*val + siz.top;
					v.push_back(t);
				}
				t.pos.x = -val + siz.right;
				t.pos.y = val + siz.top;
				v.push_back(t);
				t.pos.x = -val + siz.right;
				t.pos.y = siz.top;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//right top
				{
					t.pos.x = -val + siz.right;
					t.pos.y = val + siz.top;
					v.push_back(t);
					t.pos.x = -(val + cos(-PI - (f / val)*PI_HALF)*val) + siz.right;
					t.pos.y = val + sin(PI + (f / val)*PI_HALF)*val + siz.top;
					v.push_back(t);
				}
				t.pos.x = - val + siz.right;
				t.pos.y = - val + siz.bottom;
				v.push_back(t);
				t.pos.x = siz.right;
				t.pos.y = - val + siz.bottom;
				v.push_back(t);
				for (float f = 0.0f; f <= val; f += 1.0f)//right bottom
				{
					t.pos.x = - val + siz.right;
					t.pos.y = - val + siz.bottom;
					v.push_back(t);
					t.pos.x = - (val + cos(-PI - (f / val)*PI_HALF)*val) + siz.right;
					t.pos.y = - val + sin((f / val)*PI_HALF)*val + siz.bottom;
					v.push_back(t);
				}
				t.pos.x = val + siz.left;
				t.pos.y = - val + siz.bottom;
				v.push_back(t);
				t.pos.x = val + siz.left;
				t.pos.y = siz.bottom;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//left bottom
				{
					t.pos.x = val + siz.left;
					t.pos.y = - val + siz.bottom;
					v.push_back(t);
					t.pos.x = (val + cos(-PI - (f / val)*PI_HALF)*val) + siz.left;
					t.pos.y = - val - sin(PI + (f / val)*PI_HALF)*val + siz.bottom;
					v.push_back(t);
				}
				t.pos.x = val + siz.left;
				t.pos.y = val + siz.top;
				v.push_back(t);
				t.pos.x = 0.0f + siz.left;
				t.pos.y = val + siz.top;
				v.push_back(t);

				t.pos.x = val + siz.left;//center
				t.pos.y = val + siz.top;
				v.push_back(t);
				t.pos.x = - val + siz.right;
				t.pos.y = val + siz.top;
				v.push_back(t);
				t.pos.x = val + siz.left;
				t.pos.y = - val + siz.bottom;
				v.push_back(t);
				t.pos.x = - val + siz.right;
				t.pos.y = - val + siz.bottom;
				v.push_back(t);
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
				v.push_back(t);
			v[1].uv.x = v[3].uv.x = v[0].uv.y = v[1].uv.y = 1.0f;
			v[0].pos.x = v[2].pos.x = siz.left;
			v[0].pos.y = v[1].pos.y = siz.top;
			v[1].pos.x = v[3].pos.x = siz.right;
			v[2].pos.y = v[3].pos.y = siz.bottom;
			if (v[0].pos.x > v[1].pos.x)
				v[1].pos.x = v[3].pos.x = v[0].pos.x;
			if (v[0].pos.y > v[2].pos.y)
				v[2].pos.y = v[3].pos.y = v[0].pos.y;
		}
	}
	void WidgetBlock::_Update(map<string, string> &inherit)
	{
		_UpdateTransform(inherit);
		_UpdateBorders(inherit);
		_UpdateBackground(inherit);
	}
};