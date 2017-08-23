#include "stdafx.h"
#include "WidgetImage.h"
#include "../../Common/String.h"
#include "../../Render/Direct3d11.h"
#include "../../Material/Pass.h"
#include "../../Material/Texture.h"
namespace DEN
{
	WidgetImage::WidgetImage(GUI *gui): WidgetBlock(gui)
	{
		_img = nullptr;
		_prop["display"] = "inline";
	}
	void WidgetImage::_Render(Pass *pass)
	{
		Render *render = Render::Get();
		Texture *tex = pass->GetTexture(0);
		pass->SetTexture(0, _img);
		render->RenderPass(pass);
		render->ExecuteMesh(_buffer);
		pass->SetTexture(0, tex);
	}
	void WidgetImage::_UpdateAtlas(map<string, string> &inherit)
	{
		if(!_img)
			return;
		uint width = _img->GetWidth();
		uint height = _img->GetHeight();
		if(!width || !height)
			return;
		string style = GetProperty("atlas");
		if(style == "")
			return;
		vector<string> vars = explode(style, ' ', true);
		if(vars.size() >= 2)
		{
			float posX = (float)atof(vars[0].c_str());
			float posY = (float)atof(vars[1].c_str());
			float sizX, sizY;
			if(vars.size() >= 4)
			{
				sizX = (float)atof(vars[2].c_str());
				sizY = (float)atof(vars[3].c_str());
				_size.x = sizX;
				_size.y = sizY;
			}
			else
			{
				sizX = _rect.right;
				sizY = _rect.bottom;
			}
			_uv.minX = posX/(float)width;
			_uv.maxY = 1.0f - posY/(float)height;
			_uv.maxX = _uv.minX + sizX/(float)width;
			_uv.minY = 1.0f - (_uv.minY + sizY/(float)height);
		}
		if(_parent)
		{
			Vector2 size = _parent->GetSize();

		}
	}
	void WidgetImage::_Update(map<string, string> &inherit)
	{
		_UpdateAtlas(inherit);
		WidgetBlock::_Update(inherit);
	}
	void WidgetImage::SetImage(Texture *img)
	{
		_img = img;
		_size.x = (float)_img->GetWidth();
		_size.y = (float)_img->GetHeight();
	}
};
