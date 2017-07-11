#pragma once
#include "../Input/Input.h"
namespace DEN
{
	class WidgetX;
	class GUIListener:public InputListener
	{
	private:
		WidgetX *z_element;
		bool z_isHover;
	public:
		GUIListener(WidgetX *el);
		function<bool(MouseEvent)> onHover = nullptr;
		function<bool(MouseEvent)> onOver = nullptr;
		function<bool(MouseEvent)> onOut = nullptr;
		function<bool(MouseEventClick)> onClick = nullptr;
		function<void()> onChange = nullptr;
		function<void()> onUpdate = nullptr;
		function<bool()> onRender = nullptr;
		bool OnMouseHit(const MouseEventClick &arg);
		bool OnMouseMove(const MouseEvent &arg);
		void SetElement(WidgetX *el);
		WidgetX *GetElement();
	};
};