#include "stdafx.h"
#include "Widget.h"
#include "GUIListener.h"
#include "GUI.h"
namespace DEN
{
	GUIListener::GUIListener(Widget *el)
	{
		_data = el;
		z_isHover = false;
	}
	bool GUIListener::OnMouseHit(const MouseEventClick &arg)
	{
		if(!_data)
			return false;
		Widget *el = (Widget*)_data;
		if(el->Pick(Point2(arg.x, arg.y), true) && el->GetGUI()->IsPickChild(el))
		{
			if(onClick)
				onClick(arg, this);
		}
		return false;
	}
	bool GUIListener::OnMouseMove(const MouseEvent &arg)
	{
		if(!_data)
			return false;
		Widget *el = (Widget*)_data;
		if(el->Pick(Point2(arg.x, arg.y), true) && el->GetGUI()->IsPickChild(el))
		{
			if(!z_isHover && onOver)
					onOver(arg, this);
			z_isHover = true;
			if(onHover)
				onHover(arg, this);
			/*if(_element->IsEvent("hover"))
			{
				_element->AddEvent("hover");
				if(onHover)
					onHover(arg);
			}*/
		}
		else
		{
			/*if(_element->IsEvent("hover"))
				_element->RemoveEvent("hover");*/
			if(z_isHover)
			{
				if(onOut)
					onOut(arg, this);
				z_isHover = false;
			}
		}
		return false;
	}
	void GUIListener::SetElement(Widget *el)
	{
		_data = el;
	}
	Widget *GUIListener::GetElement()
	{
		return (Widget*)_data;
	}
};