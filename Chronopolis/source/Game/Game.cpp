#include "DC.h"
#include "../Toolset/Class.h"
#include "../Player/Player.h"
#include "../World/World.h"
#include "../GUI/GUIPlayer.h"
#include "../GUI/GUIPlan.h"
#include "Date.h"
#include "Game.h"
Game *Game::_this = nullptr;

ActorRoad::ActorRoad()
{
	vector<Sector> *v = new vector<Sector>;
	v->push_back({Vector(0.0f, 0.0f, 0.0f), 7.5f});
	v->push_back({Vector(1000.0f, 0.0f, 0.0f), 7.5f});
	AddVariable("points", v);
}
ActorRoad::~ActorRoad()
{

}
void ActorRoad::Generate(Pass *pass)
{
	Paramesh *pm = new Paramesh();
	vector<Sector> *v = (vector<Sector>*)GetVariable("points");
	pm->Begin(Game::Get()->GetInputLayout());
	uint size = (uint)v->size();
	if(size > 1)
		for(uint i = 0; i < size - 1u; ++i)
		{
			Sector v0 = v->at(i);
			Sector v1 = v->at(i + 1u);
			Vector p0 = Vector(v0.point.x, v0.point.y, v0.point.z - v0.width);
			Vector p1 = Vector(v0.point.x, v0.point.y, v0.point.z + v0.width);
			Vector p2 = Vector(v1.point.x, v1.point.y, v1.point.z + v1.width);
			Vector p3 = Vector(v1.point.x, v1.point.y, v1.point.z - v1.width);
			pm->GetMesh()->AddQuad(p0, p1, p2, p3);
		}
	pm->GetMesh()->SetColor(Color(1.0f, 1.0f, 1.0f));
	pm->End();
	pm->GetMesh()->SetMaterial(pass);
	Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
	AddComponent("mesh", pm);
	AddComponent("node", pm->GetMesh()->GetNode());
}

ActorBuild::ActorBuild()
{
	AddVariable("floors", new int(5));
	AddVariable("width", new float(12));
	AddVariable("length", new float(65));
	AddVariable("height", new float(13));
}
ActorBuild::~ActorBuild()
{

}
void ActorBuild::Detalize(uint num, Pass *pass)
{
	float width = *(float*)GetVariable("width");
	float length = *(float*)GetVariable("length");
	float height = *(float*)GetVariable("height");
	switch(num)
	{
	case 0:
	{
		Paramesh *pm = new Paramesh();
		pm->Begin(Game::Get()->GetInputLayout());
		pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(width, 0.0f, 0.0f), Vector(width, 0.0f, length), Vector(0.0f, 0.0f, length));
		pm->GetMesh()->AddQuad(Vector(0.0f, height, 0.0f), Vector(0.0f, height, length), Vector(width, height, length), Vector(width, height, 0.0f));

		pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, height, 0.0f), Vector(width, height, 0.0f), Vector(width, 0.0f, 0.0f));
		pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, length), Vector(width, 0.0f, length), Vector(width, height, length), Vector(0.0f, height, length));

		pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, length), Vector(0.0f, height, length), Vector(0.0f, height, 0.0f));
		pm->GetMesh()->AddQuad(Vector(width, 0.0f, 0.0f), Vector(width, height, 0.0f), Vector(width, height, length), Vector(width, 0.0f, length));

		pm->GetMesh()->SetColor(Color(1.0f, 1.0f, 1.0f));
		pm->End();
		pm->GetMesh()->SetMaterial(pass);
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
		//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
		AddComponent("mesh", pm);
		AddComponent("node", pm->GetMesh()->GetNode());
	}
	break;
	case 1:
	{
		Paramesh *pe = new Paramesh;
		float iWidth = 2.0f;//ширина секции с окном
		float iHeight = 2.5f;//высота секции с окном
		float iwWidth = 1.0f;//ширина окна
		float iwHeight = 1.3f;//высота окна
		float iwFoot = 1.0f;//нижний отступ окна
		float iWall = 0.5f;//толщина несущей стены
		float pad = (iWidth - iwWidth)*0.5f;
		pe->Begin(Game::Get()->GetInputLayout());
		pe->GetMesh()->AddVertex(Vector(0.0f, 0.0f, 0.0f));
		pe->GetMesh()->AddVertex(Vector(0.0f, iHeight, 0.0f));
		pe->GetMesh()->AddVertex(Vector(0.0f, iHeight, iWidth));
		pe->GetMesh()->AddVertex(Vector(0.0f, 0.0f, iWidth));

		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, iWidth - pad));
		pe->GetMesh()->AddQuad(0, 1, 5, 4);
		pe->GetMesh()->AddQuad(1, 2, 6, 5);
		pe->GetMesh()->AddQuad(2, 3, 7, 6);
		pe->GetMesh()->AddQuad(3, 0, 4, 7);

		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, pad));

		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, pad));

		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot + iwHeight, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, iWidth - pad));

		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(0.0f, iwFoot, pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, iWidth - pad));

		pe->GetMesh()->AddQuad(8, 9, 10, 11);
		pe->GetMesh()->AddQuad(12, 13, 14, 15);
		pe->GetMesh()->AddQuad(16, 17, 18, 19);
		pe->GetMesh()->AddQuad(20, 21, 22, 23);

		pe->GetMesh()->AddVertex(Vector(-iWall, 0.0f, 0.0f));
		pe->GetMesh()->AddVertex(Vector(-iWall, 0.0f, iWidth));
		pe->GetMesh()->AddVertex(Vector(-iWall, iHeight, iWidth));
		pe->GetMesh()->AddVertex(Vector(-iWall, iHeight, 0.0f));

		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, iWidth - pad));
		pe->GetMesh()->AddVertex(Vector(-iWall, iwFoot + iwHeight, pad));
		pe->GetMesh()->AddQuad(24, 25, 29, 28);
		pe->GetMesh()->AddQuad(25, 26, 30, 29);
		pe->GetMesh()->AddQuad(26, 27, 31, 30);
		pe->GetMesh()->AddQuad(27, 24, 28, 31);

		pe->GetMesh()->SetColor(Color(1.0f, 1.0f, 1.0f));
		pe->GetMesh()->GenerateNormals();
		pe->GetMesh()->GenerateUV();

		Paramesh *pm = new Paramesh;
		pm->Begin(Game::Get()->GetInputLayout());
		Matrix m;
		for(int h = 0; h < 5; ++h)
			for(int i = 0; i < 24; ++i)
			{
				m.SetOrienation(Quaternion::IDENTITY);
				m.SetTranslation(Vector(0.0f, 2.5f*(float)h, 2.0f*(float)i));
				pm->Connect(pe, m);

				m.SetOrienation(Quaternion(PI, Vector::ONE_Y));
				m.SetTranslation(Vector(-10.0f, 2.5f*(float)h, 2.0f*(float)(i + 1)));
				pm->Connect(pe, m);
			}
		for(int h = 0; h < 5; ++h)
			for(int i = 0; i < 5; ++i)
			{
				m.SetOrienation(Quaternion(PI_HALF, Vector::ONE_Y));
				m.SetTranslation(Vector(-2.0f*(float)(i + 1), 2.5f*(float)h, 0.0f));
				pm->Connect(pe, m);

				m.SetOrienation(Quaternion(-PI_HALF, Vector::ONE_Y));
				m.SetTranslation(Vector(-2.0f*(float)i, 2.5f*(float)h, 2.0f*24.0f));
				pm->Connect(pe, m);
			}
		pm->GetMesh()->GenerateNormals();
		pm->End();
		pm->GetMesh()->SetMaterial(pass);
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
		AddComponent("mesh", pm);
		AddComponent("node", pm->GetMesh()->GetNode());
	}
	break;
	case 2:
	{

	}
	break;
	}
}

void ActorBuild::Generate(Pass *pass)
{
	Detalize(1, pass);
}

ActorTerrain::ActorTerrain()
{
	AddVariable("floors", new int(5));
	AddVariable("width", new float(12));
	AddVariable("length", new float(65));
	AddVariable("height", new float(13));
}
ActorTerrain::~ActorTerrain()
{

}

void ActorTerrain::DiamondSquare()
{
	vector<Vector> vPos = _pos;
	_pos.clear();
	vector<Vector> top;
	vector<Vector> bot;
	for(uint h = 0; h < _rowCount; ++h)
		for(uint i = 0; i < _rowCount; ++i)
		{
			Vector v0 = vPos[_rowCount*h + i];
			Vector v1 = vPos[_rowCount*h + i + 1];
			Vector v2 = vPos[_rowCount*(h + 1) + i];
			Vector v3 = vPos[_rowCount*(h + 1) + i + 1];
			Vector c = (v0 + v1 + v2 + v3)*0.25f;
			Vector x0 = Vector(c.x, v0.y, (v0.z - v1.z)*0.5f);
			Vector x1 = Vector(v1.x, c.y, (v1.z - v2.z)*0.5f);
			Vector x2 = Vector(c.x, v2.y, (v2.z - v3.z)*0.5f);
			Vector x3 = Vector(v3.x, c.y, (v3.z - v0.z)*0.5f);
			
		}
	_rowCount *= 2;
}

void ActorTerrain::Generate(Pass *pass)
{
	Paramesh *pm = new Paramesh();
	float width = *(float*)GetVariable("width");
	float length = *(float*)GetVariable("length");
	float height = *(float*)GetVariable("height");

	Vector p;
	_rowCount = 1;
	_pos.push_back(Vector(-100.0f, 0.0f, -100.0f));
	_pos.push_back(Vector(-100.0f, 0.0f, 100.0f));
	_pos.push_back(Vector(100.0f, 0.0f, 100.0f));
	_pos.push_back(Vector(100.0f, 0.0f, -100.0f));
	DiamondSquare();
	pm->Begin(Game::Get()->GetInputLayout());



	pm->GetMesh()->SetColor(Color(1.0f, 1.0f, 1.0f));
	pm->End();
	pm->GetMesh()->SetMaterial(pass);
	Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
	//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
	AddComponent("mesh", pm);
	AddComponent("node", pm->GetMesh()->GetNode());
}

ActorGlass::ActorGlass()
{
	AddVariable("width", new float(1.0f));
	AddVariable("weight", new float(0.01f));
	AddVariable("height", new float(1.0f));
}

ActorGlass::~ActorGlass()
{

}
void ActorGlass::Generate(Pass *pass)
{
	float width = *(float*)GetVariable("width");
	float weight = *(float*)GetVariable("weight");
	float height = *(float*)GetVariable("height");
	Paramesh *pm = new Paramesh();
	pm->Begin(Game::Get()->GetInputLayout());
	pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(width, 0.0f, 0.0f), Vector(width, 0.0f, weight), Vector(0.0f, 0.0f, weight));
	pm->GetMesh()->AddQuad(Vector(0.0f, height, 0.0f), Vector(0.0f, height, weight), Vector(width, height, weight), Vector(width, height, 0.0f));

	pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, height, 0.0f), Vector(width, height, 0.0f), Vector(width, 0.0f, 0.0f));
	pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, weight), Vector(width, 0.0f, weight), Vector(width, height, weight), Vector(0.0f, height, weight));

	pm->GetMesh()->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, weight), Vector(0.0f, height, weight), Vector(0.0f, height, 0.0f));
	pm->GetMesh()->AddQuad(Vector(width, 0.0f, 0.0f), Vector(width, height, 0.0f), Vector(width, height, weight), Vector(width, 0.0f, weight));

	pm->GetMesh()->SetColor(Color(0.5f, 0.75f, 1.0f));
	pm->End();
	pm->GetMesh()->SetMaterial(pass);
	Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
	//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
	AddComponent("mesh", pm);
	AddComponent("node", pm->GetMesh()->GetNode());
}

ActorWindow::ActorWindow()
{
	AddVariable("width", new float(1.0f));
	AddVariable("weight", new float(0.01f));
	AddVariable("height", new float(1.3f));
	AddVariable("pad", new float(0.01f));//отступ от краев
	AddVariable("center", new float(0.01f));//разделитель центральной части
}

ActorWindow::~ActorWindow()
{

}
void ActorWindow::Generate(Pass *pass)
{
	float width = *(float*)GetVariable("width");
	float weight = *(float*)GetVariable("weight");
	float height = *(float*)GetVariable("height");
	float pad = *(float*)GetVariable("pad");
	float center = *(float*)GetVariable("center");
	float hTop = height*0.4f;

	Node *node = new Node();
	ActorGlass *glass = new ActorGlass;
	glass->SetVariable("width", new float(width*0.5f - pad*4.0f));
	glass->SetVariable("height", new float(height - pad*2.0f));
	glass->Generate(pass);
	((Node*)glass->GetComponent("node"))->SetPosition(Vector(width*0.5f + pad, 0.0f, 0.0f));
	((Node*)glass->GetComponent("node"))->SetParent(node, true);
	AddComponent("glass", glass);

	ActorGlass *glassTop = new ActorGlass;
	glassTop->SetVariable("width", new float(width*0.5f - pad*2.0f));
	glassTop->SetVariable("height", new float(hTop - pad*2.0f));
	glassTop->Generate(pass);
	((Node*)glassTop->GetComponent("node"))->SetPosition(Vector(0.0f, height - pad*2.0f - hTop - pad*2.0f, 0.0f));
	((Node*)glassTop->GetComponent("node"))->SetParent(node, true);
	AddComponent("glassTop", glassTop);

	node->Update();
	AddComponent("node", node);
}

Game::Game()
{
	_engine = new Engine();
	_this = this;
	_toolset = nullptr;
	_clock = new Clock();
	_clock->SetSpeed(60.0f);
}
Game::~Game()
{
	delete _clock;
	if(_toolset)
		delete _toolset;
	delete _engine;
}

Skill *Game::CreateSkill(uint id, const string &name)
{
	Skill *skill = new Skill();
	skill->SetId(id);
	skill->SetName(name);
	_skills[id] = skill;
	return skill;
}

void Game::InitSkills()
{
	Skill *skill = CreateSkill(1, "Walk");
}

void Game::Init2()
{
	_engine->Create(1024, 768, false);
	Manager *man = _engine->GetManager();
	Font *font = man->LoadFont("Verdana", 22);
	Scene *sc = _engine->CreateScene();
	sc->SetBackground(Color(64, 64, 64));
	ia = man->GetInputLayout("default");
	Light *li = sc->CreateLight();
	li->SetRange(1000.0f);
	li->GetNode()->SetPosition(Vector(5.0f, 5.0f, -2.0f));

	sc->GetCamera()->SetPosition(Vector(0.0f, 5.0f, 0.0f));
	sc->GetCamera()->SetRotation(Quaternion(90.0_deg, Vector(1.0f, 0.0f, 0.0f)));
	sc->GetCamera()->SetTargetEnable(true);
	sc->GetCamera()->GetTarget()->Rotate(Quaternion(-45.0_deg, Vector(0.0f, 1.0f, 0.0f))*Quaternion(-45.0_deg, Vector(1.0f, 0.0f, 0.0f)));
	sc->GetCamera()->Update();
	sc->GetCamera()->CreateListener();
	_player = new Player();
	_player->Initialize(ColorRGB(41, 128, 185).ToColor());

	Pass *pass = man->CreatePass();
	VertexShader *vs = man->CreateVS();
	vs->CompileFile(L"vs.txt", "mainVS");
	pass->SetVS(vs);
	PixelShader *ps = man->CreatePS();
	ps->CompileFile(L"vs.txt", "difPS");
	pass->SetPS(ps);

	Mesh *m = Manager::Get()->LoadMesh("box.fbx");
	if(m)
	{
		m->SetMaterial(pass);
		sc->AddMesh(m);
	}

	_guiPlayer = new GUIPlayer(sc);
	_guiPlayer->Initialize();

	Mesh *plane = new Mesh(Game::Get()->GetInputLayout());
	plane->AddQuad(Vector(-1000.0f, 0.0f, -1000.0f), Vector(-1000.0f, 0.0f, 1000.0f), Vector(1000.0f, 0.0f, 1000.0f), Vector(1000.0f, 0.0f, -1000.0f));
	plane->SetVertexColor(Color::C_GREEN);
	plane->SetMaterial(pass);
	plane->SetPickable(true);
	Game::Get()->GetEngine()->GetScene()->AddMesh(plane);

	InputListener *lis = new InputListener();
	GetEngine()->GetInput()->AddListener(lis);
	Load();
	lis->onKeyHit = [this](KeyEvent key, InputListener *lis)
	{
		GetEngine()->Exit();
		return false;
	};
}

void Game::Load()
{
	File file;
	file.Open("Save.sav", FILE_READ);
	if(!file.IsOpen())
		return;
	int version = 0;
	Vector v;
	Quaternion q;
	file.Read(version);
	if(version >= 1)
	{
		file.Read(v);
		file.Read(q);
		_player->SetPosition(v);
		_player->SetRotation(q);
	}
	file.Close();
}
void Game::Save()
{
	File file;
	file.Open("Save.sav", FILE_WRITE);
	file.Write(1);
	file.Write(_player->GetPosition());
	file.Write(_player->GetRotation());
	file.Close();
}

void Game::Update()
{
	while(_engine->Update())
	{
		WidgetX *v1 = _engine->GetScene()->GetGUI()->GetElementById("v1");
		float s = _engine->GetTime().spf;
		_clock->Update(s);
		_player->Update();
		_guiPlayer->Update(_player);
		//Scene::Stats stats = _engine->GetScene()->stats;
		//v1->GetProperty().SetInnerText("FPS: " + to_string((int)_engine->GetTime().fps) + " " + to_string(stats.vertexCount) + " " + _clock->GetText(false));
		_engine->Draw();
	}
	Save();
}