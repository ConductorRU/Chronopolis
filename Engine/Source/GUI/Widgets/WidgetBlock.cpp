#include "stdafx.h"
#include "WidgetBlock.h"
#include "../GUI.h"
#include "../../Math/Func.h"
namespace DEN
{
	WidgetBlock::WidgetBlock(GUI *gui): Widget(gui)
	{

	}
	void WidgetBlock::_UpdateBorders(map<string, string> &inherit)
	{
		string prop;
		Vertex2D t;
		float val;
		memset(&t, 0, sizeof(Vertex2D));
		prop = _GetStyle("border-radius", inherit);
		if (prop != "")
		{
			val = GetPixel("border-radius", prop);
			if (val > 0)
			{
				v.clear();
				val = min(val, min(_size.x, _size.y));
				for (float f = 0.0f; f <= val; f += 1.0f)//left top
				{
					t.pos.x = val + _rect.left;
					t.pos.y = val + _rect.top;
					v.push_back(t);
					t.pos.x = val + cos(-PI - (f / val)*PI_HALF)*val + _rect.left;
					t.pos.y = val - sin((f / val)*PI_HALF)*val + _rect.top;
					v.push_back(t);
				}
				t.pos.x = _size.x - val - _rect.right;
				t.pos.y = val + _rect.top;
				v.push_back(t);
				t.pos.x = _size.x - val - _rect.right;
				t.pos.y = _rect.top;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//right top
				{
					t.pos.x = _size.x - val - _rect.right;
					t.pos.y = val + _rect.top;
					v.push_back(t);
					t.pos.x = _size.x - (val + cos(-PI - (f / val)*PI_HALF)*val) - _rect.right;
					t.pos.y = val + sin(PI + (f / val)*PI_HALF)*val + _rect.top;
					v.push_back(t);
				}
				t.pos.x = _size.x - val - _rect.right;
				t.pos.y = _size.y - val - _rect.bottom;
				v.push_back(t);
				t.pos.x = _size.x - _rect.right;
				t.pos.y = _size.y - val - _rect.bottom;
				v.push_back(t);
				for (float f = 0.0f; f <= val; f += 1.0f)//right bottom
				{
					t.pos.x = _size.x - val - _rect.right;
					t.pos.y = _size.y - val - _rect.bottom;
					v.push_back(t);
					t.pos.x = _size.x - (val + cos(-PI - (f / val)*PI_HALF)*val) - _rect.right;
					t.pos.y = _size.y - val + sin((f / val)*PI_HALF)*val - _rect.bottom;
					v.push_back(t);
				}
				t.pos.x = val + _rect.left;
				t.pos.y = _size.y - val - _rect.bottom;
				v.push_back(t);
				t.pos.x = val + _rect.left;
				t.pos.y = _size.y - _rect.bottom;
				v.push_back(t);
				for (float f = val; f >= 0.0f; f -= 1.0f)//left bottom
				{
					t.pos.x = val + _rect.left;
					t.pos.y = _size.y - val - _rect.bottom;
					v.push_back(t);
					t.pos.x = (val + cos(-PI - (f / val)*PI_HALF)*val) + _rect.left;
					t.pos.y = _size.y - val - sin(PI + (f / val)*PI_HALF)*val - _rect.bottom;
					v.push_back(t);
				}
				t.pos.x = val + _rect.left;
				t.pos.y = val + _rect.top;
				v.push_back(t);
				t.pos.x = 0.0f + _rect.left;
				t.pos.y = val + _rect.top;
				v.push_back(t);

				t.pos.x = val + _rect.left;//center
				t.pos.y = val + _rect.top;
				v.push_back(t);
				t.pos.x = _size.x - val - _rect.right;
				t.pos.y = val + _rect.top;
				v.push_back(t);
				t.pos.x = val + _rect.left;
				t.pos.y = _size.y - val - _rect.bottom;
				v.push_back(t);
				t.pos.x = _size.x - val - _rect.right;
				t.pos.y = _size.y - val - _rect.bottom;
				v.push_back(t);
			}
		}
		else
		{
			for (int i = 0; i < 4; ++i)
				v.push_back(t);
			v[1].uv.x = v[3].uv.x = v[2].uv.y = v[3].uv.y = 1.0f;
			v[0].pos.x = v[2].pos.x = _rect.left;
			v[0].pos.y = v[1].pos.y = _rect.top;
			v[1].pos.x = v[3].pos.x = _size.x - _rect.right;
			v[2].pos.y = v[3].pos.y = _size.y - _rect.bottom;
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