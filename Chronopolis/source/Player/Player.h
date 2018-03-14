#pragma once
class Skill
{
private:
	uint _id;
	string _name;
	vector<Skill*> _parents;
public:
	inline void SetName(const string &name) {_name = name;}
	inline void SetId(uint id) {_id = id;}
	string GetName() {return _name;};
	uint GetId() {return _id;};
	Skill() {};
	~Skill() {};
};

struct SkillStat
{
	Skill *skill;
	uint level;
	float difficulty;
	float current;//0-1
	SkillStat()
	{
		skill = nullptr;
		level = 0;
		current = 0.0f;
		difficulty = 1.0f;
	}
};

struct PlayerStats
{
	int hp;
	float speed;
};
class Player: public Node
{
private:
	PlayerStats _basicStats;
	PlayerStats _maxStats;
	PlayerStats _curStats;
	Vector _targetPos;
	bool _isTargetPos;
public:
	Player();
	~Player();
	void Initialize(const Color &color);
	void OnMove();
	void Update();
};