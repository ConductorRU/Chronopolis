#pragma once
#include "../Widget.h"
#include "../Widgets/WidgetBlock.h"
namespace DEN
{
	class WidgetImage : public WidgetBlock
	{
	protected:
		Texture *_img;
		void _Render(Pass *pass);
		void _Update(map<string, string> &inherit);
		void _UpdateAtlas(map<string, string> &inherit);
	public:
		WidgetImage(GUI *gui);
		void SetImage(Texture *img);
	};
};