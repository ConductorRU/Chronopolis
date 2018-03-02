#pragma once
#include "../Input/Input.h"
namespace DEN
{
	class Widget;
	class GUIListener:public InputListener
	{
	private:
		bool z_isHover;
	public:
		GUIListener(Widget *el);
		function<bool(MouseEvent, InputListener*)> onHover = nullptr;
		function<bool(MouseEvent, InputListener*)> onOver = nullptr;
		function<bool(MouseEvent, InputListener*)> onOut = nullptr;
		function<bool(MouseEventClick, InputListener*)> onClick = nullptr;
		function<void(InputListener*)> onChange = nullptr;
		function<void(InputListener*)> onUpdate = nullptr;
		function<bool(InputListener*)> onRender = nullptr;
		bool OnMouseHit(const MouseEventClick &arg);
		bool OnMouseMove(const MouseEvent &arg);
		void SetElement(Widget *el);
		Widget *GetElement();
	};
};