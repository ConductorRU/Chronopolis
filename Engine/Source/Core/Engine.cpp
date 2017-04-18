#include "stdafx.h"
#include "../Math/Color.h"
#include "../Math/Rect.h"
#include "../Material/Atlas.h"
#include "../Render/Render.h"
#include "../Input/Input.h"
#include "../Scene/Scene.h"
#include "../Manager/Manager.h"
#include "Engine.h"
namespace DEN
{
	Engine *Engine::_this = nullptr;
	Time::Time()
	{
		fps = 0.0f;
		isStart = false;
		last = static_cast <float> (clock()) / static_cast <float> (CLOCKS_PER_SEC);
		sec = 0;
		fpsNum = 0.0f;
		fNum = 0;
		spf = 1.0f / 60.0f;
	}
	void Time::Update()
	{
		float t = static_cast <float> (clock()) / static_cast <float> (CLOCKS_PER_SEC);
		int i = clock() / CLOCKS_PER_SEC;
		if(i != sec)
		{
			++fNum;
			sec = i;
			fpsNum += 1 / (t - last);
			fps = fpsNum / float(fNum);
			fps = fNum;
			fpsNum = 0.0f;
			fNum = 0;
		}
		else
		{
			++fNum;
			fpsNum += 1 / (t - last);
		}
		spf = (t - last);
		last = t;
	}
	Engine::Engine()
	{
		_this = this;
		_hWindow = nullptr;
		_hInst = nullptr;
		_hDC = nullptr;
		_exit = false;
		_stop = false;
		_cursor = NULL;
		_cursorType = CURSOR_NONE;
		_render = nullptr;
		ZeroMemory(&_message, sizeof(_message));
		_active = true;
		_render = new Render;
		_atlas = new Atlas();
		_input = nullptr;
		_scene = nullptr;
	}
	Engine::~Engine()
	{
		delete _atlas;
		delete _render;
		if(_manager)
			delete _manager;
		for(Scene *scene: _scenes)
			delete scene;
		if(_input)
			delete _input;
	}
	Scene *Engine::CreateScene()
	{
		_scene = new Scene();
		_scenes.insert(_scene);
		return _scene;
	}
	void Engine::UpdateWindowState()
	{
		RECT ClientRect;
		ClientRect.left = _rect.minX;
		ClientRect.top = _rect.minY;
		ClientRect.right = _rect.maxX;
		ClientRect.bottom = _rect.maxY;
		if(_input)
			_input->SetWinRect(ClientRect);
	}
	LRESULT Engine::WindProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch(message)
		{
		case WM_CREATE:
			return 0;
		case WM_MOVE:
			_rect.minX = LOWORD(lParam);
			_rect.minY = HIWORD(lParam);
			UpdateWindowState();
			return 0;
		case WM_QUIT:
		case WM_CLOSE:
			_exit = true;
			return false;
		case WM_SIZE:
			_rect.maxX = LOWORD(lParam);
			_rect.maxY = HIWORD(lParam);
			m_width = _rect.maxX;
			m_height = _rect.maxY;
			UpdateWindowState();
			//if(_render && _render->GetDevice().Get())
			//	_render->UpdateSize();
			return 0;
		case WM_MOUSEMOVE: case WM_LBUTTONUP: case WM_LBUTTONDOWN: case WM_MBUTTONUP: case WM_MBUTTONDOWN: case WM_RBUTTONUP: case WM_RBUTTONDOWN: case WM_MOUSEWHEEL: case WM_KEYDOWN: case WM_KEYUP:
			if(_input)
				_input->Run(message, wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	LRESULT CALLBACK Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return Engine::Get()->WindProc(hWnd, msg, wParam, lParam);
		//AddMessage(hWnd, msg, wParam, lParam);
		/*switch(msg)
		{
		case WM_CLOSE:
			PostQuitMessage(0);
			Engine::Get()->Exit();
			return false;
		case WM_DESTROY:
			PostQuitMessage(0);
			Engine::Get()->Exit();
			return false;
		case WM_SETFOCUS:
			Engine::Get()->Continue();
			return true;
			case WM_SIZE:
			{
			int ix = GetManager()->GetEngine()->GetDevice()->GetScreenWidth();
			int iy = GetManager()->GetEngine()->GetDevice()->GetScreenHeight();
			if(wParam == SIZE_MAXIMIZED)
			GetManager()->GetEngine()->Maximize(1, 1);
			//else if(wParam == SIZE_RESTORED)
			//	GetManager()->GetEngine()->SetWindow(800, 600, 400, 300, false);
			}
			return true;
		case WM_KILLFOCUS:
			Engine::Get()->Stop();
			return true;
		case WM_ACTIVATE:
			if(LOWORD(wParam) == WA_INACTIVE)
				Engine::Get()->Stop();
			else
				Engine::Get()->Continue();
			return true;
		case WM_SYSKEYDOWN:
			if(wParam == VK_F4)
			{
				PostQuitMessage(0);
				return false;
			}
			//Engine::Get()->GetTime()->ResetTime();
			return true;
		default:
			return DefWindowProc(hWnd, msg, wParam, lParam);
		}*/
	}
	void Engine::SetCursor(CURSOR_TYPE id)
	{
		if(_cursorType == id)
			return;
		_cursorType = id;
		switch(id)
		{
		case CURSOR_ARROW: _cursor = LoadCursor(NULL, IDC_ARROW); break;
		case CURSOR_TEXT: _cursor = LoadCursor(NULL, IDC_IBEAM); break;
		case CURSOR_WAIT: _cursor = LoadCursor(NULL, IDC_WAIT); break;
		case CURSOR_CROSS: _cursor = LoadCursor(NULL, IDC_CROSS); break;
		case CURSOR_UPARROW: _cursor = LoadCursor(NULL, IDC_UPARROW); break;
		case CURSOR_DIAGONAL1: _cursor = LoadCursor(NULL, IDC_SIZENWSE); break;
		case CURSOR_DIAGONAL2: _cursor = LoadCursor(NULL, IDC_SIZENESW); break;
		case CURSOR_HORIZONTAL: _cursor = LoadCursor(NULL, IDC_SIZEWE); break;
		case CURSOR_VERTICAL: _cursor = LoadCursor(NULL, IDC_SIZENS); break;
		case CURSOR_SIZE: _cursor = LoadCursor(NULL, IDC_SIZEALL); break;
		case CURSOR_POINTER: _cursor = LoadCursor(NULL, IDC_HAND); break;
		}
		::SetCursor(_cursor);
	}
	bool Engine::IsExit()
	{
		return _exit;
	}
	void Engine::Exit()
	{
		_exit = true;
	}
	void Engine::Stop()
	{
		if(_stop)
			return;
		_stop = true;
	}
	void Engine::Continue()
	{
		if(!_stop)
			return;
		_stop = false;
	}
	void Engine::Create(int sizeX, int sizeY, bool fullscreen, const wstring &name)
	{
		srand((uint)time(NULL));
		int sx = GetSystemMetrics(SM_CXSCREEN);
		int sy = GetSystemMetrics(SM_CYSCREEN);
		if(sizeX > sx - 20)
			sizeX = sx - 20;
		if(sizeY > sy - 100)
			sizeY = sy - 100;
		m_width = sizeX;
		m_height = sizeY;
		_wClass.cbSize = sizeof(WNDCLASSEX);
		_wClass.style = CS_CLASSDC;
		_wClass.lpfnWndProc = &Engine::WndProc;
		_wClass.cbClsExtra = 0L;
		_wClass.cbWndExtra = 0L;
		_wClass.hInstance = GetModuleHandle(NULL);
		_wClass.hIcon = LoadIcon(_wClass.hInstance, MAKEINTRESOURCE(101));
		_wClass.hCursor = NULL;
		_wClass.hbrBackground = NULL;
		_wClass.lpszMenuName = NULL;
		_wClass.lpszClassName = L"DEN";
		_wClass.hIconSm = LoadIcon(_wClass.hInstance, MAKEINTRESOURCE(101));
		RegisterClassEx(&_wClass);
		DWORD style = 0;
		if(!fullscreen)
			style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP | WS_THICKFRAME | WS_MAXIMIZE | WS_MAXIMIZEBOX;
		_hWindow = CreateWindowW(L"DEN", name.c_str(), style, 0, 0, sizeX, sizeY, NULL, NULL, _wClass.hInstance, NULL);
		_hDC = GetDC(_hWindow);
		WINDOWINFO info;
		GetWindowInfo(_hWindow, &info);
		if(!_render->Initialize(_hWindow, sizeX, sizeY, fullscreen))
		{
			//Error("Device Error", "Device initialize is failed.");
		}
		SetWindowPos(_hWindow, NULL, 0, 0, info.rcWindow.right + info.rcClient.left + (info.rcWindow.right - info.rcClient.right), info.rcWindow.bottom + info.rcClient.top + (info.rcWindow.bottom - info.rcClient.bottom), SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		if(!fullscreen)
		{
			//int posX = (z_device->GetScreenWidth() - sizeX) / 2;
			//int posY = (z_device->GetScreenHeight() - sizeY - 100) / 2;
			//if(posX < 0) posX = 0;
			//if(posY < 0) posY = 0;
			//SetWindowPos(_hWindow, NULL, posX, posY, info.rcWindow.right + info.rcClient.left + (info.rcWindow.right - info.rcClient.right), info.rcWindow.bottom + info.rcClient.top + (info.rcWindow.bottom - info.rcClient.bottom), SWP_NOZORDER | SWP_SHOWWINDOW | SWP_FRAMECHANGED);
		}
		_input = new Input();
		_manager = new Manager;
		_input->Init();
		_hInst = GetModuleHandle(NULL);
		ShowWindow(_hWindow, SW_SHOWNORMAL);
		if(!fullscreen)
			_render->SetWindow(sizeX, sizeY, sizeX, sizeY, false, _hWindow);
		SetForegroundWindow(_hWindow);
		SetCursor(CURSOR_ARROW);
	}
	bool Engine::Update()
	{
		//z_msg.clear();
		if(_exit)
			return false;
		while(PeekMessage(&_message, _hWindow, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&_message);
			DispatchMessage(&_message);
		}
		if(_stop == false)
		{
			_input->Update();
			//z_time->UpdateFPS();
			_time.Update();
		}
		/*else
			z_time->ResetTime();*/
		return true;
	}
	void Engine::Reload()
	{
		_render->Reset();
		_active = true;
	}
	void Engine::Begin()
	{
		if(_stop)
			return;
		//z_manager->Merge(0.01f);
		if(!_active)
			Reload();
		//Sleep(1);
	}

	void Engine::End()
	{
		_render->End();
	}
	void Engine::Draw()
	{
		Begin();
		if(_active)
		{
			for(Scene *scene: _scenes)
			{
				if (scene->GetRenderTarget())
				{
					_render->SetRenderTarget(scene->GetRenderTarget());
					_render->Begin(scene->GetBackground());
					scene->Render();
				}
			}
			for(Scene *scene : _scenes)
			{
				if(!scene->GetRenderTarget())
				{
					_render->SetRenderTarget(nullptr);
					_render->Begin(scene->GetBackground());
					scene->Render();
				}
			}
			_render->End();
		}
		End();
	}
	void Engine::Run()
	{

	}
	Vector2 Engine::GetCursorPos()
	{
		Point2 pos = _input->GetCursorPos();
		return Vector2(1.0f - 2.0f*(float)pos.x / (float)_render->GetWidth(), 1.0f - 2.0f*(float)pos.y / (float)_render->GetHeight());
	}
}