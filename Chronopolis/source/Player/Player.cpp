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
	_script->func["onCreate"] = [this](char **)
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
		AddComponent("mesh", pm);
		AddComponent("node", pm->GetMesh()->GetNode());

		InputListener *input = (InputListener*)act->GetVariable("input");
		Engine::Get()->GetInput()->AddListener(input);
		input->onMouseHit = [act, this](MouseEventClick m)
		{
			Node *node = (Node*)act->GetComponent("node");
			Vector pos;
			Vector nor = Vector::ONE_Y;
			Vector inter = Vector();
			float dist = 0.0f;
			if (Game::Get()->GetEngine()->GetScene()->GetCamera()->IntersectPlane(pos, nor, inter, dist))
				SetVariable("move", new Vector(inter));
			return false;
		};
		return act;
	};
	_script->func["onMove"] = [this](char **v)
	{
		Vector *pos = (Vector*)GetVariable("move");
		Vector start = ((Node*)GetComponent("node"))->GetPosition(true);
		if(pos)
		{
			float spf = Engine::Get()->GetTime().spf;
			Vector v = (*pos - start);
			Vector n = v;
			n.Normalize();
			Vector d = n*spf;
			if (d.Length() > v.Length())
			{
				((Node*)GetComponent("node"))->SetPosition(*pos);
				DeleteVariable("move");
			}
			else
			{
				Vector res = start + d;
				((Node*)GetComponent("node"))->SetPosition(res);
			}
		}
		return nullptr;
	};
	_script->func["onTick"] = [this](char **v)
	{
		float spf = Engine::Get()->GetTime().spf;
		_script->func["onMove"](nullptr);
		return nullptr;
	};
	Actor *act = (Actor*)_script->func["onCreate"](nullptr);

}
void Player::Update()
{
	if(_script->func.find("onTick") != _script->func.end())
		_script->func["onTick"](nullptr);
}