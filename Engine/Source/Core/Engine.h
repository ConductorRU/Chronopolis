#pragma once
namespace DEN
{
	class Rect;
	class Scene;
	class Input;
	enum CURSOR_TYPE
	{
		CURSOR_NONE = 0,
		CURSOR_ARROW = 1,
		CURSOR_TEXT = 2,
		CURSOR_WAIT = 3,
		CURSOR_CROSS = 4,
		CURSOR_UPARROW = 5,
		CURSOR_DIAGONAL1 = 6,
		CURSOR_DIAGONAL2 = 7,
		CURSOR_HORIZONTAL = 8,
		CURSOR_VERTICAL = 9,
		CURSOR_SIZE = 10,
	};
	struct Time
	{
	private:
		float fpsNum;
	public:
		int fNum;
		int sec;
		float last;
		float fps;
		float spf;
		bool isStart;
		Time();
		void Update();
	};
	class Engine
	{
	private:
		WNDCLASSEX _wClass;
		HINSTANCE _hInst;
		HWND _hWindow;
		HDC _hDC;
		MSG _message;
		HCURSOR _cursor;
		CURSOR_TYPE _cursorType;
		bool _exit;
		bool _stop;
		bool _active;
		UINT m_width;// Viewport dimensions.
		UINT m_height;
		Rect _rect;
		static Engine *_this;
		LRESULT WindProc(HWND hwnd, UINT nMsg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void UpdateWindowState();
		Render *_render;
		Input *_input;
		Time _time;
		Scene *_scene;
	public:
		Input *GetInput() { return _input; };
		static Engine *Get() { return _this; };
		Render *GetRender() { return _render; };
		Scene *GetScene() { return _scene; };
		const Time &GetTime() { return _time; };
		Engine();
		~Engine();
		Scene *CreateScene();
		void SetCursor(CURSOR_TYPE id);
		bool IsExit();
		void Exit();
		void Stop();
		void Continue();
		void Create(int sizeX, int sizeY, bool fullscreen = false, const wstring &name = L"DEN");
		bool Update();
		void Run();
		void Reload();
		void Begin();
		void End();
		void Draw();
	};
}