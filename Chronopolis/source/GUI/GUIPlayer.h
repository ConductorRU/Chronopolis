#pragma once
class GUIPlayer
{
private:
	Scene *_scene;
public:
	GUIPlayer(Scene *scene);
	~GUIPlayer();
	void Initialize();
};