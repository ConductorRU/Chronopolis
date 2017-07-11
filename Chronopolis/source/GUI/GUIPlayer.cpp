#include "DC.h"
#include "GUIPlayer.h"
GUIPlayer::GUIPlayer(Scene *scene)
{
	_scene = scene;
}
GUIPlayer::~GUIPlayer()
{

}
void GUIPlayer::Initialize()
{
	GUI *gui = _scene->GetGUI();
	/*WidgetX *root = gui->GetRoot();
	WidgetX *main = root->CreateChild();
	main->SetStyle("width:100%;height:160px;background-color:#aaa;align:bottom;");
	WidgetX *bar = main->CreateChild("");
	bar->SetStyle("x:10px;y:10px;background-color:#ddd;width:10px;height:100%-20px;");
	WidgetX *barL = bar->CreateChild();
	barL->SetStyle("background-color:#66f;width:100%;height:60%;align:bottom;");*/
}