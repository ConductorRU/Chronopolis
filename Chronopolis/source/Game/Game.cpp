#include "DC.h"
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
	uint size = v->size();
	if(size > 1)
		for(uint i = 0; i < size - 1u; ++i)
		{
			Sector v0 = v->at(i);
			Sector v1 = v->at(i + 1u);
			Vector p0 = Vector(v0.point.x, v0.point.y, v0.point.z - v0.width);
			Vector p1 = Vector(v0.point.x, v0.point.y, v0.point.z + v0.width);
			Vector p2 = Vector(v1.point.x, v1.point.y, v1.point.z + v1.width);
			Vector p3 = Vector(v1.point.x, v1.point.y, v1.point.z - v1.width);
			pm->AddQuad(p0, p1, p2, p3);
		}
	pm->SetColor(Color(1.0f, 1.0f, 1.0f));
	pm->End();
	pm->GetMesh()->SetMaterial(pass);
	Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
	AddComponent("mesh", pm);
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
void ActorBuild::Generate(Pass *pass)
{
	Paramesh *pm = new Paramesh();
	float width = *(float*)GetVariable("width");
	float length = *(float*)GetVariable("length");
	float height = *(float*)GetVariable("height");
	pm->Begin(Game::Get()->GetInputLayout());
	pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(width, 0.0f, 0.0f), Vector(width, 0.0f, length), Vector(0.0f, 0.0f, length));
	pm->AddQuad(Vector(0.0f, height, 0.0f), Vector(0.0f, height, length), Vector(width, height, length), Vector(width, height, 0.0f));

	pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, height, 0.0f), Vector(width, height, 0.0f), Vector(width, 0.0f, 0.0f));
	pm->AddQuad(Vector(0.0f, 0.0f, length), Vector(width, 0.0f, length), Vector(width, height, length), Vector(0.0f, height, length));

	pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, length), Vector(0.0f, height, length), Vector(0.0f, height, 0.0f));
	pm->AddQuad(Vector(width, 0.0f, 0.0f), Vector(width, height, 0.0f), Vector(width, height, length), Vector(width, 0.0f, length));

	pm->SetColor(Color(1.0f, 1.0f, 1.0f));
	pm->End();
	pm->GetMesh()->SetMaterial(pass);
	Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
	//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
	AddComponent("mesh", pm);
}


Game::Game()
{
	_engine = new Engine();
	_this = this;
}
Game::~Game()
{
	delete _engine;
}

void Game::Init()
{
	_engine->Create(1024, 768, false);
	Manager *man = _engine->GetManager();
	Scene *sc = _engine->CreateScene();
	ia = man->CreateInputLayout();
	ia->Add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ia->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ia->Add("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
	Light *li = sc->CreateLight();
	li->GetNode()->SetPosition(Vector(0.0f, 10.0f, -2.0f));
	//sc->GetCamera()->SetPosition(Vector(0.0f, 1.0f, -2.0f));
	sc->GetCamera()->SetPosition(Vector(0.0f, 5.0f, 0.0f));
	sc->GetCamera()->SetRotation(Quaternion(AngleToRad(90.0f), Vector(1.0f, 0.0f, 0.0f)));
	sc->GetCamera()->SetTargetEnable(true);
	sc->GetCamera()->GetTarget()->Rotate(Quaternion(AngleToRad(-45.0f), Vector(0.0f, 1.0f, 0.0f))*Quaternion(AngleToRad(-45.0f), Vector(1.0f, 0.0f, 0.0f)));
	sc->GetCamera()->Update();
	sc->GetCamera()->CreateListener();
	Pass *pass = man->CreatePass();
	VertexShader *vs = man->CreateVS();
	vs->CompileFile(L"vs.txt", "mainVS");
	pass->SetVS(vs);
	PixelShader *ps = man->CreatePS();
	ps->CompileFile(L"vs.txt", "mainPS");
	pass->SetPS(ps);


	ActorBuild *build = new ActorBuild;;
	build->Generate(pass);
	ActorRoad *road = new ActorRoad;;
	road->Generate(pass);
	Paramesh *pm = man->CreateParamesh();
	gm = pm->Generate(sc, ia, 0);
	gm->SetMaterial(pass);

	Paramesh *mp = man->CreateParamesh();
	mp->Generate(sc, ia, 1)->SetMaterial(pass);

	Random r1(2);
	r1.Generate(100);
	uint r2 = 0;
	Vector pos;
	for(uint i: r1.nums)
	{
		srand(i);
		Paramesh *mp = man->CreateParamesh();
		Mesh *m1 = mp->Generate(sc, ia, 0);
		m1->SetMaterial(pass);
		float v1 = Rand(-10.0f, 10.0f);
		float v2 = Rand(-10.0f, 10.0f);
		float v3 = Rand(-10.0f, 10.0f);
		pos = Vector(v1, 0.0f, v3);
		m1->GetNode()->SetPosition(pos);
	}
}

void Game::Update()
{
	while(_engine->Update())
	{
		float s = _engine->GetTime().spf;
		gm->GetNode()->Rotate(Quaternion(s*0.8f, Vector::ONE_Y)*Quaternion(s*0.3f, Vector::ONE_X)*Quaternion(s*0.4f, Vector::ONE_Z));
		_engine->Draw();
	}
}