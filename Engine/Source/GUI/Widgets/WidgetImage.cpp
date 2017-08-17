#include "stdafx.h"
#include "WidgetImage.h"
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
	void WidgetImage::SetImage(Texture *img)
	{
		_img = img;
		if(_prop.find("width") == _prop.end())
			_prop["width"] = to_string(_img->GetTextureDesc().Width) + "px";
		if(_prop.find("height") == _prop.end())
			_prop["height"] = to_string(_img->GetTextureDesc().Height) + "px";
	}
};
