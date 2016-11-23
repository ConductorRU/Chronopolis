#pragma once
#include "../Math/Point2.h"
namespace DEN
{
	class Point2;
	enum MOUSE_TYPE
	{
		MOUSE_LEFT = 0,
		MOUSE_MIDDLE,
		MOUSE_RIGHT,
		MOUSE_MAX
	};
	enum KEY_TYPE
	{
		KEY_LBUTTON = 0x01, // Left mouse button
		KEY_RBUTTON = 0x02, // Right mouse button
		KEY_CANCEL = 0x03, // Control-break processing
		KEY_MBUTTON = 0x04, // Middle mouse button (three-button mouse)
		KEY_XBUTTON1 = 0x05, // X1 mouse button
		KEY_XBUTTON2 = 0x06, // X2 mouse button
		KEY_BACK = 0x08, // BACKSPACE key
		KEY_TAB = 0x09, // TAB key
		KEY_CLEAR = 0x0C, // CLEAR key
		KEY_RETURN = 0x0D, // ENTER key
		KEY_SHIFT = 0x10, // SHIFT key
		KEY_CONTROL = 0x11, // CTRL key
		KEY_ALT = 0x12, // ALT key
		KEY_PAUSE = 0x13, // PAUSE key
		KEY_CAPITAL = 0x14, // CAPS LOCK key
		KEY_KANA = 0x15, // IME Kana mode
		KEY_HANGUEL = 0x15, // IME Hanguel mode
		KEY_HANGUL = 0x15, // IME Hangul mode
		KEY_JUNJA = 0x17, // IME Junja mode
		KEY_FINAL = 0x18, // IME final mode
		KEY_HANJA = 0x19, // IME Hanja mode
		KEY_KANJI = 0x19, // IME Kanji mode

		KEY_ESCAPE = 0x1B,
		KEY_SPACE = 0x20,
		KEY_PAGEUP = 0x21,
		KEY_PAGEDOWN = 0x22,
		KEY_END = 0x23,
		KEY_HOME = 0x24,
		KEY_LEFT = 0x25, // left arrow key
		KEY_UP = 0x26, // up arrow key
		KEY_RIGHT = 0x27, // right arrow key
		KEY_DOWN = 0x28, // down arrow key
		KEY_SELECT = 0x29,
		KEY_EXE = 0x2B, // execute key
		KEY_SNAPSHOT = 0x2C,
		KEY_INSERT = 0x2D,
		KEY_DELETE = 0x2E,
		KEY_HELP = 0x2F,

		KEY_0 = 0x30,
		KEY_1 = 0x31,
		KEY_2 = 0x32,
		KEY_3 = 0x33,
		KEY_4 = 0x34,
		KEY_5 = 0x35,
		KEY_6 = 0x36,
		KEY_7 = 0x37,
		KEY_8 = 0x38,
		KEY_9 = 0x39,

		KEY_A = 0x41,
		KEY_B = 0x42,
		KEY_C = 0x43,
		KEY_D = 0x44,
		KEY_E = 0x45,
		KEY_F = 0x46,
		KEY_G = 0x47,
		KEY_H = 0x48,
		KEY_I = 0x49,
		KEY_J = 0x4A,
		KEY_K = 0x4B,
		KEY_L = 0x4C,
		KEY_M = 0x4D,
		KEY_N = 0x4E,
		KEY_O = 0x4F,
		KEY_P = 0x50,
		KEY_Q = 0x51,
		KEY_R = 0x52,
		KEY_S = 0x53,
		KEY_T = 0x54,
		KEY_U = 0x55,
		KEY_V = 0x56,
		KEY_W = 0x57,
		KEY_X = 0x58,
		KEY_Y = 0x59,
		KEY_Z = 0x5A,

		KEY_WINLEFT = 0x5B,
		KEY_WINRIGHT = 0x5C,
		KEY_APPS = 0x5D,

		KEY_NUMPAD0 = 0x60,
		KEY_NUMPAD1 = 0x61,
		KEY_NUMPAD2 = 0x62,
		KEY_NUMPAD3 = 0x63,
		KEY_NUMPAD4 = 0x64,
		KEY_NUMPAD5 = 0x65,
		KEY_NUMPAD6 = 0x66,
		KEY_NUMPAD7 = 0x67,
		KEY_NUMPAD8 = 0x68,
		KEY_NUMPAD9 = 0x69,

		KEY_MULTIPLY = 0x6A,
		KEY_ADD = 0x6B,
		KEY_SEPARATOR = 0x6C,
		KEY_SUBTRACT = 0x6D,
		KEY_DECIMAL = 0x6E,
		KEY_DIVIDE = 0x6F,

		KEY_F1 = 0x70,
		KEY_F2 = 0x71,
		KEY_F3 = 0x72,
		KEY_F4 = 0x73,
		KEY_F5 = 0x74,
		KEY_F6 = 0x75,
		KEY_F7 = 0x76,
		KEY_F8 = 0x77,
		KEY_F9 = 0x78,
		KEY_F10 = 0x79,
		KEY_F11 = 0x7A,
		KEY_F12 = 0x7B,
		KEY_F13 = 0x7C,
		KEY_F14 = 0x7D,
		KEY_F15 = 0x7E,
		KEY_F16 = 0x7F,
		KEY_F17 = 0x80,
		KEY_F18 = 0x81,
		KEY_F19 = 0x82,
		KEY_F20 = 0x83,
		KEY_F21 = 0x84,
		KEY_F22 = 0x85,
		KEY_F23 = 0x86,
		KEY_F24 = 0x87,

		KEY_NUMLOCK = 0x90,
		KEY_SCROLL = 0x91,

		KEY_LSHIFT = 0xA0,
		KEY_RSHIFT = 0xA1,
		KEY_LCONTROL = 0xA2,
		KEY_RCONTROL = 0xA3,
		KEY_LALT = 0xA4,
		KEY_RALT = 0xA5,
		KEY_SEMICOLON = 0xBA,
		KEY_PLUS = 0xBB, // '+'
		KEY_COMMA = 0xBC, // ','
		KEY_MINUS = 0xBD, // '-'
		KEY_PERIOD = 0xBE, // '.'
		KEY_SLASH = 0xBF,
		KEY_GRAVE = 0xC0,

		KEY_LBRACKET = 0xDB,
		KEY_BACKSLASH = 0xDC, // '^'
		KEY_RBRACKET = 0xDD,
		KEY_APOSTROPHE = 0xDE,

		KEY_ATTN = 0xF6,
		KEY_CRSEL = 0xF7,
		KEY_EXSEL = 0xF8,
		KEY_EREOF = 0xF9,
		KEY_PLAY = 0xFA,
		KEY_ZOOM = 0xFB,
		KEY_NONAME = 0xFC,
		KEY_PA1 = 0xFD,
		KEY_OEMCLEAR = 0xFE,

		KEY_MAX = 0x100
	};

	struct MouseEvent
	{
		MouseEvent(int nx, int ny): x(nx), y(ny) {}
		int x;
		int y;
	};
	struct MouseEventClick: public MouseEvent
	{
		MouseEventClick(MOUSE_TYPE b, int nx, int ny): MouseEvent(nx, ny), btn(b) {}
		const MOUSE_TYPE btn;
	};
	struct MouseEventWheel: public MouseEvent
	{
		int wheel;
		MouseEventWheel(int nwheel, int nx, int ny): MouseEvent(nx, ny), wheel(nwheel) {}
	};
	struct KeyEvent
	{
		const wchar_t wc;
		const KEY_TYPE code;
		KeyEvent(wchar_t c, KEY_TYPE kc): wc(c), code(kc) {}
	};

	class InputListener
	{
		bool z_enable;
	public:
		InputListener();
		function<bool(MouseEventClick)> onMouseHit = nullptr;
		function<bool(MouseEventClick)> onMousePressed = nullptr;
		function<bool(MouseEventClick)> onMouseReleased = nullptr;
		function<bool(MouseEventWheel)> onMouseWhell = nullptr;
		function<bool(MouseEvent)> onMouseMove = nullptr;
		function<bool(KeyEvent)> onKeyHit = nullptr;
		function<bool(KeyEvent)> onKeyPressed = nullptr;
		function<bool(KeyEvent)> onKeyReleased = nullptr;
		function<void()> onUpdate = nullptr;
		virtual bool OnMouseHit(const MouseEventClick &arg) { return false; };
		virtual bool OnMousePressed(const MouseEventClick &arg) { return false; };
		virtual bool OnMouseReleased(const MouseEventClick &arg) { return false; };
		virtual bool OnMouseWheel(const MouseEventWheel &arg) { return false; };
		virtual bool OnMouseMove(const MouseEvent &arg) { return false; };
		virtual bool OnKeyHit(const KeyEvent &arg) { return false; };
		virtual bool OnKeyPressed(const KeyEvent &arg) { return false; };
		virtual bool OnKeyReleased(const KeyEvent &arg) { return false; };
		virtual void OnUpdate() {};
		bool MouseHit(const MouseEventClick &arg) { if(onMouseHit != nullptr) return onMouseHit(arg); return false; }
		bool MousePressed(const MouseEventClick &arg) { if(onMousePressed != nullptr) return onMousePressed(arg); return false; }
		bool MouseReleased(const MouseEventClick &arg) { if(onMouseReleased != nullptr) return onMouseReleased(arg); return false; }
		bool MouseWheel(const MouseEventWheel &arg) { if(onMouseWhell != nullptr) return onMouseWhell(arg); return false; }
		bool MouseMove(const MouseEvent &arg) { if(onMouseMove != nullptr) return onMouseMove(arg); return false; }
		bool KeyHit(const KeyEvent &arg) { if(onKeyHit != nullptr) return onKeyHit(arg); return false; }
		bool KeyPressed(const KeyEvent &arg) { if(onKeyPressed != nullptr) return onKeyPressed(arg); return false; }
		bool KeyReleased(const KeyEvent &arg) { if(onKeyReleased != nullptr) return onKeyReleased(arg); return false; }
		void Update() { if(onUpdate != nullptr) onUpdate(); }
		bool IsEnable() { return z_enable; }
		void SetEnable(bool enable) { z_enable = enable; }
	};
	class Input
	{
	private:
		void Z_OnCursor();// событие движения мыши
		void Z_OnPressed();
		void Z_OnUpdate();
		void Z_OnMouse(const MOUSE_TYPE KeyCode, bool press);// событие кнопки мыши
		void Z_OnWhell(short Value);// событие вращения колесика
		void Z_OnKey(const KEY_TYPE KeyCode, const wchar_t ch, bool press);// обработка события клавиши
		std::list<InputListener*> z_listener;
		bool z_pressed[256];
		bool z_mpressed[3];
		RECT z_rect;
		Point2 z_cursor;
		int z_whell;
	public:
		void Init();
		void Close();
		bool IsKeyPressed(KEY_TYPE code);
		bool IsMousePressed(MOUSE_TYPE code);
		Point2 GetCursorPos();
		string GetInputText(KEY_TYPE code);
		void Run(const UINT &msg, WPARAM wParam, LPARAM lParam);
		void Update();
		void AddListener(InputListener *Listener);
		void RemoveListener(InputListener *Listener);
		void SetWinRect(const RECT &winrect);// зона окна
	};
}