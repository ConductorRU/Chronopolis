#pragma once
class GUIPlayer
{
private:
	Scene *_scene;
public:
	static bool OpenStats(MouseEventClick eve, InputListener *lis);
	static bool Hover(MouseEvent eve, InputListener *lis);
	GUIPlayer(Scene *scene);
	~GUIPlayer();
	void Initialize();
	void Update(class Player *player);
};