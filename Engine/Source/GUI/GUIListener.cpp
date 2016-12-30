#include "stdafx.h"
#include "Widget.h"
#include "GUIListener.h"
#include "GUI.h"
namespace DEN
{
	GUIListener::GUIListener(Widget *el)
	{
		z_element = el;
		z_isHover = false;
	}
	bool GUIListener::OnMouseHit(const MouseEventClick &arg)
	{
		if(!z_element)
			return false;
		if(z_element->Pick(Point2(arg.x, arg.y), true) && z_element->GetGUI()->IsPickChild(z_element))
		{
			if(onClick)
				onClick(arg);
		}
		return false;
	}
	bool GUIListener::OnMouseMove(const MouseEvent &arg)
	{
		if(!z_element)
			return false;
		if(z_element->Pick(Point2(arg.x, arg.y), true) && z_element->GetGUI()->IsPickChild(z_element))
		{
			if(!z_isHover && onOver)
					onOver(arg);
			z_isHover = true;
			if(z_element->IsEvent("hover"))
			{
				z_element->AddEvent("hover");
				if(onHover)
					onHover(arg);
			}
		}
		else
		{
			if(z_element->IsEvent("hover"))
				z_element->RemoveEvent("hover");
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
		z_element = el;
	}
	Widget *GUIListener::GetElement()
	{
		return z_element;
	}
};