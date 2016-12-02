#pragma once

class ActorRoad: public Actor
{
protected:
	struct Sector
	{
		Vector point;
		float width;
	};
public:
	ActorRoad();
	~ActorRoad();
	void Generate(Pass *pass);
};

class ActorBuild: public Actor
{
protected:

public:
	ActorBuild();
	~ActorBuild();
	void Generate(Pass *pass);
};

class Game
{
private:
	Engine *_engine;
	Mesh *gm;
	InputLayout *ia;
	static Game *_this;
public:
	InputLayout *GetInputLayout() { return ia; }
	Engine *GetEngine() {return _engine;}
	static Game *Get() { return _this; }
	Game();
	~Game();
	void Init();
	void Update();
};