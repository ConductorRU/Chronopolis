#include "DC.h"
#include "Game.h"

Random::Random(uint s)
{
	seed = s;
}

void Random::Generate(uint count)
{
	srand(seed);
	for(uint i = 0; i < count; ++i)
		nums.push_back(rand());
}

Game::Game()
{
	_engine = new Engine();
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
	InputLayout *ia = man->CreateInputLayout();
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
	Paramesh *pm = man->CreateParamesh();
	gm = pm->Generate(sc, ia, 0);

	Pass *pass = man->CreatePass();
	VertexShader *vs = man->CreateVS();
	vs->CompileFile(L"vs.txt", "mainVS");
	pass->SetVS(vs);
	PixelShader *ps = man->CreatePS();
	ps->CompileFile(L"vs.txt", "mainPS");
	pass->SetPS(ps);

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