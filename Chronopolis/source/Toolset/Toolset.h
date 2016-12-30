#pragma once
class MainWindow;
class Toolset
{
private:
	Scene *_scene;
	MainWindow *_window;
public:
	Toolset(Scene *scene);
	~Toolset();
	void InitWorkspace();
};