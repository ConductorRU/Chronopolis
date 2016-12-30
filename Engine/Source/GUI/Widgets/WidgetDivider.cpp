#include "stdafx.h"
#include "WidgetDivider.h"
#include "../Widget.h"
#include "../GUIListener.h"
#include "../GUI.h"
#include "../../Input/Input.h"
#include "../../Core/Engine.h"
namespace DEN
{
	WidgetDivider::WidgetDivider(GUI *gui): Widget(gui)
	{
		_part[0] = CreateChild("el1");
		_part[0]->SetStyle("x:0;y:0;width:50%;height:100%;background-color:#faa;");
		_part[1] = CreateChild("el2");
		_part[1]->SetStyle("x:50%;y:0;width:50%;height:100%;background-color:#aaf;");
		_line = CreateChild("el3", true);
		_line->SetStyle("x:50% - 3px;y:0;width:6px;height:100%;background-color:#0003;");
		_line->GetListener()->onOver = [](MouseEvent &eve)
		{
			Engine::Get()->SetCursor(CURSOR_HORIZONTAL);
			return false;
		};
		_line->GetListener()->onOut = [this](MouseEvent &eve)
		{
			if(!_line->IsData("press"))
				Engine::Get()->SetCursor(CURSOR_ARROW);
			return false;
		};
		_line->GetListener()->onMousePressed = [this](MouseEvent &eve)
		{
			if(_line->Pick(Point2(eve.x, eve.y)))
			{
				_line->SetData("press", "1");
				_line->SetData("x", to_string(eve.x));
				return true;
			}
			return false;
		};
		_line->GetListener()->onMouseReleased = [this](MouseEvent &eve)
		{
			if(_line->IsData("press"))
				Engine::Get()->SetCursor(CURSOR_ARROW);
			_line->SetData("press", "");
			return false;
		};
		_line->GetListener()->onMouseMove = [this](MouseEvent &eve)
		{
			_line->GetListener()->OnMouseMove(eve);
			if(_line->IsData("press"))
			{
				int x = atoi(_line->GetData("x").c_str());
				int v = eve.x;
				int w = _line->GetProperty().GetSquare().maxX;
				int hw = w/2;
				if(v < hw)
					v = hw;
				else if(v > _line->GetParent()->GetProperty().GetSquare().maxX - hw)
					v = _line->GetParent()->GetProperty().GetSquare().maxX - hw;
				_line->GetParent()->GetByName("el1")->SetStyle("width:" + to_string(v) + "px");
				int width = _line->GetParent()->GetProperty().GetSquare().maxX - v;
				_line->GetParent()->GetByName("el2")->SetStyle("x:" + to_string(v) + "px;width:" + to_string(width) + "px");
				_line->SetStyle("x:" + to_string(v - hw) + "px");
				return true;
			}
			return false;
		};
	}
	WidgetDivider::~WidgetDivider()
	{

	}
}