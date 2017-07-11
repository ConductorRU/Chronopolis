#include "DC.h"
#include "MainWindow.h"
MainWindow::MainWindow(GUI *gui)
{
	_gui = gui;
}
MainWindow::~MainWindow()
{
	Clear();
}
void MainWindow::Initialize()
{
	WidgetDivider *div = new WidgetDivider(_gui);
	//div->SetParent(_gui->GetRoot());
	div->SetStyle("width:100%;height:100%;background-color:#aaa;");
}
void MainWindow::Clear()
{

}