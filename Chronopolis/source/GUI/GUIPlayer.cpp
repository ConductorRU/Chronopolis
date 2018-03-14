#include "DC.h"
#include "GUIPlayer.h"
bool GUIPlayer::OpenStats(MouseEventClick eve, InputListener *lis)
{
	Widget *w = (Widget*)lis->GetData();

	return true;
}
bool GUIPlayer::Hover(MouseEvent eve, InputListener *lis)
{
	Widget *w = (Widget*)lis->GetData();
	w->SetBackgroundColor(Color("#f00"));
	return true;
}
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
	Widget *block = new WidgetBlock(gui);
	block->SetParent(gui->GetRoot());
	block->SetAlign(WIDGET_BOTTOM_STRETCH);
	block->SetHeight(32.0f);
	block->SetBackgroundColor(Color("#eee"));//#999

	Widget *child = new WidgetBlock(gui);
	child->SetParent(block);
	child->SetAlign(WIDGET_TOP_RIGHT);
	child->SetTop(0.0f);
	child->SetRight(3.0f);
	child->SetWidth(80.0f);
	child->SetHeight(29.0f);
	child->SetBackgroundColor(Color("#ddd"));

	WidgetImage *iblock = new WidgetImage(gui);
	iblock->SetParent(child);
	iblock->SetBackgroundColor(Color::C_WHITE);
	iblock->SetAtlas(Square(0, 0, 10, 10));

	GUIListener *lis = iblock->CreateListener();
	lis->onMouseHit = GUIPlayer::OpenStats;
	lis->onHover = GUIPlayer::Hover;
	Texture *bTex = Manager::Get()->LoadTexture("..\\..\\menu.tga", true);
	iblock->SetImage(bTex);
}

void GUIPlayer::Update(Player *player)
{

}