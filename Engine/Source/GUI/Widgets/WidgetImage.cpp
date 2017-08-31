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
		_atlas = nullptr;
	}
	WidgetImage::~WidgetImage()
	{
		delete _atlas;
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
	void WidgetImage::_UpdateAtlas()
	{
		if(!_img)
			return;
		uint width = _img->GetWidth();
		uint height = _img->GetHeight();
		if(!width || !height)
			return;
		if(!_atlas)
			return;
		_size.x = _atlas->maxX;
		_size.y = _atlas->maxY;
		_uv.minX = _atlas->minX/(float)width;
		_uv.maxY = 1.0f - _atlas->minY/(float)height;
		_uv.maxX = _uv.minX + _atlas->maxX/(float)width;
		_uv.minY = 1.0f - (_uv.minY + _atlas->maxY/(float)height);
		if(_parent)
		{
			Vector2 size = _parent->GetSize();
		}
	}
	void WidgetImage::_Update()
	{
		_UpdateAtlas();
		WidgetBlock::_Update();
	}
	void WidgetImage::SetImage(Texture *img)
	{
		_img = img;
		_size.x = (float)_img->GetWidth();
		_size.y = (float)_img->GetHeight();
	}
	void WidgetImage::SetAtlas(const Square &rect)
	{
		if(!_atlas)
			_atlas = new Square(rect);
		else
			*_atlas = rect;
	}
	Square *WidgetImage::GetAtlas()
	{
		return _atlas;
	}
};
