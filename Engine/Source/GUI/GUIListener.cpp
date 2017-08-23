#include "stdafx.h"
#include "Widget.h"
#include "GUIListener.h"
#include "GUI.h"
namespace DEN
{
	GUIListener::GUIListener(Widget *el)
	{
		_element = el;
		z_isHover = false;
	}
	bool GUIListener::OnMouseHit(const MouseEventClick &arg)
	{
		if(!_element)
			return false;
		if(_element->Pick(Point2(arg.x, arg.y), true) && _element->GetGUI()->IsPickChild(_element))
		{
			if(onClick)
				onClick(arg);
		}
		return false;
	}
	bool GUIListener::OnMouseMove(const MouseEvent &arg)
	{
		if(!_element)
			return false;
		if(_element->Pick(Point2(arg.x, arg.y), true) && _element->GetGUI()->IsPickChild(_element))
		{
			if(!z_isHover && onOver)
					onOver(arg);
			z_isHover = true;
			if(onHover)
				onHover(arg);
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
					onOut(arg);
				z_isHover = false;
			}
		}
		return false;
	}
	void GUIListener::SetElement(Widget *el)
	{
		_element = el;
	}
	Widget *GUIListener::GetElement()
	{
		return _element;
	}
};