#include "DC.h"
#include "MainWindow.h"
#include "Toolset.h"

Toolset::Toolset(Scene *scene)
{
	_scene = scene;
	_window = nullptr;
}

Toolset::~Toolset()
{
	if(_window)
		delete _window;
}

void Toolset::InitWorkspace()
{
	_window = new MainWindow(_scene->GetGUI());
	_window->Initialize();
}