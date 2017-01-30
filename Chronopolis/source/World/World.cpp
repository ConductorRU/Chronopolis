#include "DC.h"
#include "World.h"
#include "../Game/Game.h"

World::World()
{

}
World::~World()
{

}
void World::Initialize()
{
	_script = new ActorScript;
	_script->func["onCreate"] = [this](char **)
	{
		Manager *man = Engine::Get()->GetManager();
		Pass *pass = man->CreatePass();
		VertexShader *vs = man->CreateVS();
		vs->CompileFile(L"vs.txt", "mainVS");
		pass->SetVS(vs);
		PixelShader *ps = man->CreatePS();
		ps->CompileFile(L"vs.txt", "mainPS");
		pass->SetPS(ps);
		Texture *tex = new Texture();
		tex->SetAddress(TEXTURE_ADDRESS_WRAP);
		tex->Create(512, 512);
		tex->Lock(TEXTURE_LOCK_WRITE_DISCARD);
		for (uint x = 0; x < 512; ++x)
			for (uint y = 0; y < 512; ++y)
			{
				ColorRGB col = ColorRGB::C_WHITE;
				if ((((x / 32) % 2) && !((y / 32) % 2)) || (!((x / 32) % 2) && ((y / 32) % 2)))
					col = ColorRGB(180, 180, 180);
				tex->SetPixel(x, y, col);
			}
		tex->Unlock();
		//tex->Clear(ColorRGB(0, 127, 255));
		pass->SetTexture(0, tex);

		Actor *act = new Actor();
		act->AddVariable("width", new float(1.0_m));
		act->AddVariable("height", new float(1.0_m));
		act->AddVariable("length", new float(1.0_m));
		act->AddVariable("input", new InputListener);

		float width = *(float*)act->GetVariable("width");
		float length = *(float*)act->GetVariable("length");
		float height = *(float*)act->GetVariable("height");
		Paramesh *pm = new Paramesh();
		pm->Begin(Game::Get()->GetInputLayout());
		float h = 0.0f;
		int num = 50;
		Perlin2D p;

		for (int x = 0; x <= num; ++x)
			for (int y = 0; y <= num; ++y)
			{
				h = p.Noise(x/50.0f, y / 50.0f, 3)*10.0f;
				uint id = pm->AddVertex(Vector(x*width, h , y*length));
				Vector *nor = (Vector*)((&pm->GetMesh()->GetVertex(id - 1u)[28]));
				nor->y = 1;
			}
		for (int x = 0; x < num; ++x)
			for (int y = 0; y < num; ++y)
			{
				//pm->AddTriangle(y*num + x, y * num + x + 1, (y + 1)* num + x);
				pm->AddTriangle(x*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y);
				pm->AddTriangle((x + 1)*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y + 1);
			}
		pm->SetColor(ColorRGB(0, 179, 142).ToColor());
		//pm->SetColor(ColorRGB(255_r, 255_r, 255_r).ToColor());
		pm->End();
		pm->GenerateNormals();
		pm->GetMesh()->SetMaterial(pass);
		pm->GetMesh()->SetPickable(true);
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
		//Mesh *gm = pm->Generate(Game::Get()->GetEngine()->GetScene(), Game::Get()->GetInputLayout(), 0);
		AddComponent("mesh", pm);
		AddComponent("node", pm->GetMesh()->GetNode());
		pm->GetMesh()->GetNode()->SetPosition(Vector(-25.0, 0.0f, -25.0f));
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
	_script->func["onTick"] = [this](char **v)
	{
		float spf = Engine::Get()->GetTime().spf;
		
		return nullptr;
	};
	Actor *act = (Actor*)_script->func["onCreate"](nullptr);

}
void World::Update()
{
	if (_script->func.find("onTick") != _script->func.end())
		_script->func["onTick"](nullptr);
}