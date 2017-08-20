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
			float posX = atof(vars[0].c_str());
			float posY = atof(vars[1].c_str());
			float sizX, sizY;
			if(vars.size() >= 4)
			{
				sizX = atof(vars[2].c_str());
				sizY = atof(vars[3].c_str());
			}
			else
			{
				
			}
		}
	}
	void WidgetImage::_Update(map<string, string> &inherit)
	{
		WidgetBlock::_Update(inherit);
		_UpdateAtlas(inherit);
	}
	void WidgetImage::SetImage(Texture *img)
	{
		_img = img;
		if(_prop.find("width") == _prop.end())
			_prop["width"] = to_string(_img->GetTextureDesc().Width) + "px";
		if(_prop.find("height") == _prop.end())
			_prop["height"] = to_string(_img->GetTextureDesc().Height) + "px";
	}
};
