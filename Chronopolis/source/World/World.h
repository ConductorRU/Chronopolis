#pragma once

class Mineral : public Actor
{
private:

public:
	Mineral();
	~Mineral();
	void Initialize();
	void Update();
};

class World : public Actor
{
private:

public:
	World();
	~World();
	void Initialize();
	void Update();
};