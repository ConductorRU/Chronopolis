#pragma once
struct PlayerStats
{
	int hp;
	float speed;
};
class Player: public Actor
{
private:
	PlayerStats _basicStats;
	PlayerStats _maxStats;
	PlayerStats _curStats;
	ActorScript *_script;
public:
	Player();
	~Player();
	void Initialize();
	void Update();
};