#include "DC.h"
#include "World.h"
#include "../Game/Game.h"

Mineral::Mineral()
{

}

Mineral::~Mineral()
{

}

void Mineral::Initialize()
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
		ps->CompileFile(L"vs.txt", "difPS");
		pass->SetPS(ps);

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
		pm->GetMesh()->GetNode()->SetPosition(Vector::ONE_X);

		return act;
	};
	Actor *act = (Actor*)_script->func["onCreate"](nullptr);
}

void Mineral::Update()
{

}

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

		float h = 0.0f;
		int num = 50;
		Perlin2 p(10);
		Perlin2 p1(102);
		Perlin2 p2(7);
		Paramesh *pm = nullptr;
		for(int ix = -1; ix <= 1; ++ix)
			for (int iy = -1; iy <= 1; ++iy)
			{
				pm = new Paramesh();
				pm->Begin(Game::Get()->GetInputLayout());
				for (int x = 0; x <= num; ++x)
					for (int y = 0; y <= num; ++y)
					{
						h = p.Noise(x / 50.0f + ix, y / 50.0f + iy, 5)*10.0f;
						h += p1.Noise(x / 50.0f + ix, y / 50.0f + iy, 3)*10.0f;
						h += p2.Noise((x / 50.0f + ix + 1)/50.0f, (y / 50.0f + iy + 1) / 50.0f, 5)*1000.0f;
						uint id = pm->AddVertex(Vector(x*width, h, y*length));
						Vector *nor = (Vector*)((&pm->GetMesh()->GetVertex(id - 1u)[28]));
						nor->y = 1;
					}
				for (int x = 0; x < num; ++x)
					for (int y = 0; y < num; ++y)
					{
						pm->AddTriangle(x*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y);
						pm->AddTriangle((x + 1)*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y + 1);
					}
				pm->SetColor(ColorRGB(0, 179, 142).ToColor());
				pm->End();
				pm->GenerateNormals();
				pm->GetMesh()->SetMaterial(pass);
				pm->GetMesh()->SetPickable(true);
				Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
				pm->GetMesh()->GetNode()->SetPosition(Vector(-25.0 + 50.0f*ix, 0.0f, -25.0f + 50.0f*iy));
			}
		pm = new Paramesh();
		pm->Begin(Game::Get()->GetInputLayout());
		for (int x = 0; x <= num; ++x)
			for (int y = 0; y <= num; ++y)
			{
				h = p2.Noise(x / 50.0f, y / 50.0f, 5)*1000.0f;
				uint id = pm->AddVertex(Vector(x*width*50.0f, h, y*length*50.0f));
				Vector *nor = (Vector*)((&pm->GetMesh()->GetVertex(id - 1u)[28]));
				nor->y = 1;
			}
		for (int x = 0; x < num; ++x)
			for (int y = 0; y < num; ++y)
			{
				pm->AddTriangle(x*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y);
				pm->AddTriangle((x + 1)*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y + 1);
			}
		pm->SetColor(ColorRGB(0, 179, 142).ToColor());
		pm->End();
		pm->GenerateNormals();
		pm->GetMesh()->SetMaterial(pass);
		pm->GetMesh()->SetPickable(true);
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm->GetMesh());
		pm->GetMesh()->GetNode()->SetPosition(Vector(-25.0 - 50.0f, 0.0f, -25.0f - 50.0f));
		/*Paramesh *pm1 = new Paramesh();
		pm1->Begin(Game::Get()->GetInputLayout());
		for (int x = 0; x <= num; ++x)
			for (int y = 0; y <= num; ++y)
			{
				h = p.Noise(x / 50.0f, y / 50.0f + 1, 3)*10.0f;
				uint id = pm1->AddVertex(Vector(x*width, h, y*length));
				Vector *nor = (Vector*)((&pm1->GetMesh()->GetVertex(id - 1u)[28]));
				nor->y = 1;
			}
		for (int x = 0; x < num; ++x)
			for (int y = 0; y < num; ++y)
			{
				pm1->AddTriangle(x*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y);
				pm1->AddTriangle((x + 1)*(num + 1) + y + 1, (x + 1)*(num + 1) + y, x*(num + 1) + y + 1);
			}
		pm1->SetColor(ColorRGB(0, 179, 142).ToColor());
		pm1->End();
		pm1->GenerateNormals();
		pm1->GetMesh()->SetMaterial(pass);
		pm1->GetMesh()->SetPickable(true);
		pm1->GetMesh()->GetNode()->SetPosition(Vector(-25.0, 0.0f, 25.0f));
		Game::Get()->GetEngine()->GetScene()->AddMesh(pm1->GetMesh());*/



		Mineral *miner = new Mineral();
		miner->Initialize();
		Vector f = Vector(Rand(0.0f, 50.0f), 0.0f, Rand(0.0f, 50.0f));
		f.y = p.Noise(f.x / 50.0f, f.z / 50.0f, 3)*10.0f;
		((Node*)miner->GetComponent("node"))->SetPosition(f + Vector(-25.0, 0.0f, -25.0f));
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
