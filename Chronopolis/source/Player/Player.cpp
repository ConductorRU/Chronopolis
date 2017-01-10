#include "DC.h"
#include "Player.h"
#include "../Game/Game.h"
Player::Player()
{

}
Player::~Player()
{

}
void Player::Initialize()
{
	_basicStats.speed = 1.0f;
	_curStats = _maxStats = _basicStats;
	_script = new ActorScript;
	_script->func["onCreate"] = [](char **)
	{
		Manager *man = Engine::Get()->GetManager();
		Pass *pass = man->CreatePass();
		VertexShader *vs = man->CreateVS();
		vs->CompileFile(L"vs.txt", "mainVS");
		pass->SetVS(vs);
		PixelShader *ps = man->CreatePS();
		ps->CompileFile(L"vs.txt", "difPS");
		pass->SetPS(ps);

		Actor *act = new Actor();
		act->AddVariable("width", new float(50.0_sm));
		act->AddVariable("height", new float(180.0_sm));
		act->AddVariable("length", new float(30.0_sm));
		act->AddVariable("input", new InputListener);

		float width = *(float*)act->GetVariable("width");
		float length = *(float*)act->GetVariable("length");
		float height = *(float*)act->GetVariable("height");
		Paramesh *pm = new Paramesh();
		pm->Begin(Game::Get()->GetInputLayout());
		pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(width, 0.0f, 0.0f), Vector(width, 0.0f, length), Vector(0.0f, 0.0f, length));
		pm->AddQuad(Vector(0.0f, height, 0.0f), Vector(0.0f, height, length), Vector(width, height, length), Vector(width, height, 0.0f));

		pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, height, 0.0f), Vector(width, height, 0.0f), Vector(width, 0.0f, 0.0f));
		pm->AddQuad(Vector(0.0f, 0.0f, length), Vector(width, 0.0f, length), Vector(width, height, length), Vector(0.0f, height, length));

		pm->AddQuad(Vector(0.0f, 0.0f, 0.0f), Vector(0.0f, 0.0f, length), Vector(0.0f, height, length), Vector(0.0f, height, 0.0f));
		pm->AddQuad(Vector(width, 0.0f, 0.0f), Vector(width, height, 0.0f), Vector(width, height, length), Vector(width, 0.0f, length));

		pm->SetColor(ColorRGB(241, 179, 142).ToColor());
		//pm->SetColor(ColorRGB(255_r, 255_r, 255_r).ToColor());
		pm->End();
		pm->GetMesh()->SetMaterial(pass);
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
		//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
		act->AddComponent("mesh", pm);
		act->AddComponent("node", pm->GetMesh()->GetNode());

		InputListener *input = (InputListener*)act->GetVariable("input");
		Engine::Get()->GetInput()->AddListener(input);
		input->onMouseHit = [act](MouseEventClick m)
		{
			Node *node = (Node*)act->GetComponent("node");
			node->SetPosition(Vector(5.0f, 0.0f, 5.0f));
			return false;
		};
		return act;
	};
	_script->func["onMove"] = [](char **)
	{

		return nullptr;
	};
	Actor *act = (Actor*)_script->func["onCreate"](nullptr);

}