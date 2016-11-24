#include "DC.h"
using namespace DEN;
#pragma comment( lib, "Engine.lib" )
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine, int nCommandShow)
{
	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);
	Engine engine;
	engine.Create(1024, 768, false);
	Manager *man = engine.GetManager();
	Scene *sc = engine.CreateScene();
	InputLayout *ia = man->CreateInputLayout();
	ia->Add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
	ia->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
	ia->Add("NORMAL", DXGI_FORMAT_R32G32B32A32_FLOAT);
	Light *li = sc->CreateLight();
	li->GetNode()->SetPosition(Vector(0.0f, 0.0f, -2.0f));
	sc->GetCamera()->SetPosition(Vector(0.0f, 0.0f, -2.0f));

	Paramesh *pm = man->CreateParamesh();
	Mesh *gm = pm->Generate(sc, ia, 0);

	Pass *pass = man->CreatePass();
	VertexShader *vs = man->CreateVS();
	vs->CompileFile(L"vs.txt", "mainVS");
	pass->SetVS(vs);
	PixelShader *ps = man->CreatePS();
	ps->CompileFile(L"vs.txt", "mainPS");
	pass->SetPS(ps);

	gm->SetMaterial(pass);
	while(engine.Update())
	{
		float s = engine.GetTime().spf;
		gm->GetNode()->Rotate(Quaternion(s*0.8f, Vector::ONE_Y)*Quaternion(s*0.3f, Vector::ONE_X)*Quaternion(s*0.4f, Vector::ONE_Z));
		engine.Draw();
	}
	_CrtMemDumpAllObjectsSince(&_ms);
	return 0;
}