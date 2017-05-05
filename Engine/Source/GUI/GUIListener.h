#pragma once
#include "../Input/Input.h"
namespace DEN
{
	class Widget;
	class GUIListener:public InputListener
	{
	private:
		Widget *z_element;
		bool z_isHover;
	public:
		GUIListener(Widget *el);
		function<bool(MouseEvent)> onHover = nullptr;
		function<bool(MouseEvent)> onOver = nullptr;
		function<bool(MouseEvent)> onOut = nullptr;
		function<bool(MouseEventClick)> onClick = nullptr;
		function<void()> onChange = nullptr;
		function<void()> onUpdate = nullptr;
		function<bool()> onRender = nullptr;
		bool OnMouseHit(const MouseEventClick &arg);
		bool OnMouseMove(const MouseEvent &arg);
		void SetElement(Widget *el);
		Widget *GetElement();
	};
};