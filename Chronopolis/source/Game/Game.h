#pragma once
struct Random
{
	uint seed;
	vector<uint> nums;
	Random(uint s);
	void Generate(uint count);
};

class Game
{
private:
	Engine *_engine;
	Mesh *gm;
public:
	Engine *GetEngine() {return _engine;}
	Game();
	~Game();
	void Init();
	void Update();
};