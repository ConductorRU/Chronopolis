#pragma once
class MainWindow
{
private:
	GUI *_gui;
public:
	MainWindow(GUI *gui);
	~MainWindow();
	void Initialize();
	void Clear();
};