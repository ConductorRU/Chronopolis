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
					t.pos.x = val + _offset.left;
					t.pos.y = val + _offset.top;
					v.push_back(t);
					t.pos.x = val + cos(-PI - (f / val)*PI_HALF)*val + _offset.left;
					t.pos.y = val - sin((f / val)*PI_HALF)*val + _offset.top;
					v.push_back(t);
				}
				t.pos.x = -val + _offset.right;
				t.pos.y = val + _offset.top;
				v.push_back(t);
				t.pos.x = -val + _offset.right;
				t.pos.y = _offset.top;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//right top
				{
					t.pos.x = -val + _offset.right;
					t.pos.y = val + _offset.top;
					v.push_back(t);
					t.pos.x = -(val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.right;
					t.pos.y = val + sin(PI + (f / val)*PI_HALF)*val + _offset.top;
					v.push_back(t);
				}
				t.pos.x = - val + _offset.right;
				t.pos.y = - val + _offset.bottom;
				v.push_back(t);
				t.pos.x = _offset.right;
				t.pos.y = - val + _offset.bottom;
				v.push_back(t);
				for (float f = 0.0f; f <= val; f += 1.0f)//right bottom
				{
					t.pos.x = - val + _offset.right;
					t.pos.y = - val + _offset.bottom;
					v.push_back(t);
					t.pos.x = - (val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.right;
					t.pos.y = - val + sin((f / val)*PI_HALF)*val + _offset.bottom;
					v.push_back(t);
				}
				t.pos.x = val + _offset.left;
				t.pos.y = - val + _offset.bottom;
				v.push_back(t);
				t.pos.x = val + _offset.left;
				t.pos.y = _offset.bottom;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//left bottom
				{
					t.pos.x = val + _offset.left;
					t.pos.y = - val + _offset.bottom;
					v.push_back(t);
					t.pos.x = (val + cos(-PI - (f / val)*PI_HALF)*val) + _offset.left;
					t.pos.y = - val - sin(PI + (f / val)*PI_HALF)*val + _offset.bottom;
					v.push_back(t);
				}
				t.pos.x = val + _offset.left;
				t.pos.y = val + _offset.top;
				v.push_back(t);
				t.pos.x = 0.0f + _offset.left;
				t.pos.y = val + _offset.top;
				v.push_back(t);

				t.pos.x = val + _offset.left;//center
				t.pos.y = val + _offset.top;
				v.push_back(t);
				t.pos.x = - val + _offset.right;
				t.pos.y = val + _offset.top;
				v.push_back(t);
				t.pos.x = val + _offset.left;
				t.pos.y = - val + _offset.bottom;
				v.push_back(t);
				t.pos.x = - val + _offset.right;
				t.pos.y = - val + _offset.bottom;
				v.push_back(t);
			}
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
	}
	void WidgetBlock::_Update(map<string, string> &inherit)
	{
		_UpdatePosition(inherit);
		_UpdateBorders(inherit);
		_UpdateBackground(inherit);
	}
};