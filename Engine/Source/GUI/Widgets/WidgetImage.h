#pragma once
#include "../Widget.h"
#include "../Widgets/WidgetBlock.h"
namespace DEN
{
	class WidgetImage : public WidgetBlock
	{
	protected:
		Texture *_img;
		Square *_atlas;
		void _Render(Pass *pass);
		void _Update();
		void _UpdateAtlas();
	public:
		WidgetImage(GUI *gui);
		~WidgetImage();
		void SetImage(Texture *img);
		void SetAtlas(const Square &rect);
		Square *GetAtlas();
	};
};