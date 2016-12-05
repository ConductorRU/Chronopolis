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
	void Detalize(uint num, Pass *pass);
public:
	ActorBuild();
	~ActorBuild();
	void Generate(Pass *pass);
};

class ActorTerrain: public Actor
{
protected:
	uint _rowCount;
	vector<Vector> _pos;
	void DiamondSquare();
public:
	ActorTerrain();
	~ActorTerrain();
	void Generate(Pass *pass);
};

class ActorGlass: public Actor
{
protected:
public:
	ActorGlass();
	~ActorGlass();
	void Generate(Pass *pass);
};

class ActorWindow: public Actor
{
protected:
public:
	ActorWindow();
	~ActorWindow();
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