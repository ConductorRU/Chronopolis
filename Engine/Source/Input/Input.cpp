#include "stdafx.h"
#include "Input.h"
//#include "Log.h"

namespace DEN
{
	InputListener::InputListener(void *el)
	{
		z_enable = true;
		_data = el;
	}
	void Input::Init()
	{
		z_whell = 0;
		//Log::Get()->Debug("InputMgr init");
		memset(z_pressed, 0, sizeof(z_pressed));
		memset(z_mpressed, 0, sizeof(z_mpressed));
	}

	void Input::Close()
	{
		if(!z_listener.empty())
			z_listener.clear();
		//Log::Get()->Debug("InputMgr close");
	}
	bool Input::IsKeyPressed(KEY_TYPE code)
	{
		return z_pressed[code];
	}
	bool Input::IsMousePressed(MOUSE_TYPE code)
	{
		return z_mpressed[code];
	}
	Point2 Input::GetCursorPos()
	{
		return z_cursor;
	}
	string Input::GetInputText(KEY_TYPE code)
	{
		switch(code)
		{
		case KEY_NUMPAD1: return "1";
		case KEY_NUMPAD2: return "2";
		case KEY_NUMPAD3: return "3";
		case KEY_NUMPAD4: return "4";
		case KEY_NUMPAD5: return "5";
		case KEY_NUMPAD6: return "6";
		case KEY_NUMPAD7: return "7";
		case KEY_NUMPAD8: return "8";
		case KEY_NUMPAD9: return "9";
		case KEY_NUMPAD0: return "0";
		case KEY_SPACE:		return " ";
		case KEY_TAB:	  	return "	";
		}
		if(IsKeyPressed(KEY_SHIFT))
		{
			switch(code)
			{
			case KEY_1: return "!";
			case KEY_2: return "@";
			case KEY_3: return "#";
			case KEY_4: return "$";
			case KEY_5: return "%";
			case KEY_6: return "^";
			case KEY_7: return "&";
			case KEY_8: return "*";
			case KEY_9: return "(";
			case KEY_0: return ")";
			case KEY_MINUS: return "_";
			case KEY_PLUS:  return "+";
			case KEY_Q: return "Q";
			case KEY_W: return "W";
			case KEY_E: return "E";
			case KEY_R: return "R";
			case KEY_T: return "T";
			case KEY_Y: return "Y";
			case KEY_U: return "U";
			case KEY_I: return "I";
			case KEY_O: return "O";
			case KEY_P: return "P";
			case KEY_LBRACKET: return "{";
			case KEY_RBRACKET: return "}";
			case KEY_A: return "A";
			case KEY_S: return "S";
			case KEY_D: return "D";
			case KEY_F: return "F";
			case KEY_G: return "G";
			case KEY_H: return "H";
			case KEY_J: return "J";
			case KEY_K: return "K";
			case KEY_L: return "L";
			case KEY_SEMICOLON: return ":";
			case KEY_APOSTROPHE: return "\"";
			case KEY_Z: return "Z";
			case KEY_X: return "X";
			case KEY_C: return "C";
			case KEY_V: return "V";
			case KEY_B: return "B";
			case KEY_N: return "N";
			case KEY_M: return "M";
			case KEY_COMMA: return "<";
			case KEY_PERIOD: return ">";
			case KEY_SLASH: return "?";
			case KEY_GRAVE: return "~";
			case KEY_BACKSLASH: return "|";
			}
		}
		else switch(code)
		{
		case KEY_1: return "1";
		case KEY_2: return "2";
		case KEY_3: return "3";
		case KEY_4: return "4";
		case KEY_5: return "5";
		case KEY_6: return "6";
		case KEY_7: return "7";
		case KEY_8: return "8";
		case KEY_9: return "9";
		case KEY_0: return "0";
		case KEY_MINUS: return "-";
		case KEY_PLUS:  return "=";
		case KEY_Q: return "q";
		case KEY_W: return "w";
		case KEY_E: return "e";
		case KEY_R: return "r";
		case KEY_T: return "t";
		case KEY_Y: return "y";
		case KEY_U: return "u";
		case KEY_I: return "i";
		case KEY_O: return "o";
		case KEY_P: return "p";
		case KEY_LBRACKET: return "[";
		case KEY_RBRACKET: return "]";
		case KEY_A: return "a";
		case KEY_S: return "s";
		case KEY_D: return "d";
		case KEY_F: return "f";
		case KEY_G: return "g";
		case KEY_H: return "h";
		case KEY_J: return "j";
		case KEY_K: return "k";
		case KEY_L: return "l";
		case KEY_SEMICOLON: return ";";
		case KEY_APOSTROPHE: return "'";
		case KEY_Z: return "z";
		case KEY_X: return "x";
		case KEY_C: return "c";
		case KEY_V: return "v";
		case KEY_B: return "b";
		case KEY_N: return "n";
		case KEY_M: return "m";
		case KEY_COMMA: return ",";
		case KEY_PERIOD: return ".";
		case KEY_SLASH: return "/";
		case KEY_GRAVE: return "`";
		case KEY_BACKSLASH: return "\\";
		}
		return "";
	}
	void Input::SetWinRect(const RECT &winrect)
	{
		z_rect.left = winrect.left;
		z_rect.right = winrect.right;
		z_rect.top = winrect.top;
		z_rect.bottom = winrect.bottom;
	}

	void Input::AddListener(InputListener *Listener)
	{
		z_listener.push_back(Listener);
	}

	void Input::RemoveListener(InputListener *Listener)
	{
		z_listener.remove(Listener);
	}

	void Input::Run(const UINT &msg, WPARAM wParam, LPARAM lParam)
	{
		if(z_listener.empty())
			return;

		KEY_TYPE KeyIndex;
		wchar_t buffer[1];
		BYTE lpKeyState[256];
		switch(msg)
		{
		case WM_KEYDOWN:
			KeyIndex = static_cast<KEY_TYPE>(wParam);
			z_pressed[KeyIndex] = true;
			GetKeyboardState(lpKeyState);
			ToUnicode((uint)wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			Z_OnKey(KeyIndex, buffer[0], true);
			break;
		case WM_KEYUP:
			KeyIndex = static_cast<KEY_TYPE>(wParam);
			z_pressed[KeyIndex] = false;
			GetKeyboardState(lpKeyState);
			ToUnicode((uint)wParam, HIWORD(lParam) & 0xFF, lpKeyState, buffer, 1, 0);
			Z_OnKey(KeyIndex, buffer[0], false);
			break;
		case WM_LBUTTONDOWN:
			z_mpressed[0] = true;
			Z_OnMouse(MOUSE_LEFT, true);
			break;
		case WM_LBUTTONUP:
			z_mpressed[0] = false;
			Z_OnMouse(MOUSE_LEFT, false);
			break;
		case WM_RBUTTONDOWN:
			z_mpressed[2] = true;
			Z_OnMouse(MOUSE_RIGHT, true);
			break;
		case WM_RBUTTONUP:
			z_mpressed[2] = false;
			Z_OnMouse(MOUSE_RIGHT, false);
			break;
		case WM_MBUTTONDOWN:
			z_mpressed[1] = true;
			Z_OnMouse(MOUSE_MIDDLE, true);
			break;
		case WM_MBUTTONUP:
			z_mpressed[1] = false;
			Z_OnMouse(MOUSE_MIDDLE, false);
			break;
		case WM_MOUSEWHEEL:
			Z_OnWhell((short)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
			break;
		}
	}
	void Input::Update()
	{
		Z_OnCursor();// событие движения мыши
		Z_OnPressed();
		Z_OnUpdate();
	}
	void Input::Z_OnCursor()
	{
		POINT Position;
		::GetCursorPos(&Position);	// получаем текущую позицию курсора

		Position.x -= z_rect.left;
		Position.y -= z_rect.top;

		//if (z_cursor.x==Position.x && z_cursor.y==Position.y)
		//	return;

		z_cursor.x = Position.x;
		z_cursor.y = Position.y;

		for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
		{
			if(!(*it) || !(*it)->IsEnable())
				continue;
			else if((*it)->MouseMove(MouseEvent(z_cursor.x, z_cursor.y)) == true)
				return;
			if((*it)->OnMouseMove(MouseEvent(z_cursor.x, z_cursor.y)) == true)
				return;
		}
	}
	void Input::Z_OnPressed()
	{
		for(int i = 0; i != 256; ++i)
		{
			if(z_pressed[i])
			{
				for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
				{
					if(!(*it) || !(*it)->IsEnable())
						continue;
					if((*it)->KeyPressed(KeyEvent(0, KEY_TYPE(i))))
						return;
					if((*it)->OnKeyPressed(KeyEvent(0, KEY_TYPE(i))))
						return;
				}
			}
		}
		for(int i = 0; i != 3; ++i)
		{
			if(z_mpressed[i])
			{
				for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
				{
					if(!(*it) || !(*it)->IsEnable())
						continue;
					if((*it)->MousePressed(MouseEventClick(MOUSE_TYPE(i), z_cursor.x, z_cursor.y)))
						return;
					if((*it)->OnMousePressed(MouseEventClick(MOUSE_TYPE(i), z_cursor.x, z_cursor.y)))
						return;
				}
			}
		}
	}
	void Input::Z_OnUpdate()
	{
		for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
		{
			if(!(*it) || !(*it)->IsEnable())
				continue;
			(*it)->Update();
			(*it)->OnUpdate();
		}
	}
	void Input::Z_OnMouse(const MOUSE_TYPE Code, bool press)
	{
		for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
		{
			if(!(*it) || !(*it)->IsEnable())
				continue;
			if(press == true)
			{
				if((*it)->MouseHit(MouseEventClick(Code, z_cursor.x, z_cursor.y)) == true)
					return;
				if((*it)->OnMouseHit(MouseEventClick(Code, z_cursor.x, z_cursor.y)) == true)
					return;
			}
			else
			{
				if((*it)->MouseReleased(MouseEventClick(Code, z_cursor.x, z_cursor.y)) == true)
					return;
				if((*it)->OnMouseReleased(MouseEventClick(Code, z_cursor.x, z_cursor.y)) == true)
					return;
			}
		}
	}

	void Input::Z_OnWhell(short Value)
	{
		z_whell = Value;

		for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
		{
			if(!(*it) || !(*it)->IsEnable())
				continue;
			else if((*it)->MouseWheel(MouseEventWheel(z_whell, z_cursor.x, z_cursor.y)) == true)
				return;
			if((*it)->OnMouseWheel(MouseEventWheel(z_whell, z_cursor.x, z_cursor.y)) == true)
				return;
		}
	}

	void Input::Z_OnKey(const KEY_TYPE KeyCode, const wchar_t ch, bool press)
	{
		for(auto it = z_listener.begin(); it != z_listener.end(); ++it)
		{
			if(!(*it) || !(*it)->IsEnable())
				continue;
			if(press == true)
			{
				if((*it)->KeyHit(KeyEvent(ch, KeyCode)) == true)
					return;
				if((*it)->OnKeyHit(KeyEvent(ch, KeyCode)) == true)
					return;
			}
			else
			{
				if((*it)->KeyReleased(KeyEvent(ch, KeyCode)) == true)
					return;
				if((*it)->OnKeyReleased(KeyEvent(ch, KeyCode)) == true)
					return;
			}
		}
	}
}