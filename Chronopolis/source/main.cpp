#include "DC.h"
using namespace DEN;
#pragma comment( lib, "Engine.lib" )
struct Vertex
{
	Vector pos;
	Color col;
	Vector normal;
};
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPreviousInst, LPSTR lpCommandLine, int nCommandShow)
{
	//void *i = new int(4);
	size_t i = typeid(i).hash_code();
	auto n = typeid(i).name();
	Engine engine;
	engine.Create(1024, 768, false);
	Scene *sc = engine.CreateScene();
	InputLayout *ia = new InputLayout();
	ia->Add("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, PER_VERTEX, 0);
	ia->Add("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, PER_VERTEX, 0);
	ia->Add("NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 28, PER_VERTEX, 0);
	Light *li = sc->CreateLight();
	li->GetNode()->SetPosition(Vector(0.0f, 0.0f, -2.0f));
	Mesh *m = sc->CreateMesh(ia);
	sc->GetCamera()->SetPosition(Vector(0.0f, 0.0f, -2.0f));
	m->GetVertexBuffer()->Init(1024);
	Vertex *v = (Vertex*)m->CreateVertex();
	v->pos = Vector(0.0f, 0.5f, 0.0f);
	v->col = Color(0.0f, 0.0f, 1.0f, 1.0f);
	v->normal = Vector::MINUS_Z;
	v = (Vertex*)m->CreateVertex();
	v->pos = Vector(0.45f, -0.5f, 0.0f);
	v->col = Color(0.0f, 1.0f, 0.0f, 1.0f);
	v->normal = Vector::MINUS_Z;
	v = (Vertex*)m->CreateVertex();
	v->pos = Vector(-0.45f, -0.5f, 0.0f);
	v->col = Color(1.0f, 0.0f, 0.0f, 1.0f);
	v->normal = Vector::MINUS_Z;
	m->AddTriangle(0, 1, 2);
	m->Bake();

	Paramesh *pm = new Paramesh;
	Mesh *gm = pm->Generate(sc, ia, 0);
	Pass *pass = new Pass;
	VertexShader *vs = new VertexShader();
	vs->Compile("cbuffer MatrixBuffer"
	"{"
	"matrix world;"
	"matrix view;"
	"matrix proj;"
	"float3 lightPos;"
	"};"
	"struct VertexInputType"
	"{"
	"	float4 position : POSITION;"
	"	float4 color : COLOR;"
	"	float3 normal : NORMAL;"
	"};"
	"struct PixelInputType"
	"{"
	"	float4 position : SV_POSITION;"
	"	float4 color : COLOR;"
	"	float3 normal : TEXCOORD0;"
	"	float3 light : TEXCOORD1;"
	"};"
	"	PixelInputType main(VertexInputType input)"
	"{"
	"	PixelInputType output;"
	"	input.position.w = 1.0f;"
	"	output.position = mul(world, input.position);"
	"	output.light = lightPos - output.position.xyz;"
	"	output.position = mul(view, output.position);"
	"	output.position = mul(proj, output.position);"
	"	output.normal = mul(world, input.normal);"
	
	"	output.color = input.color;"
	"	return output;"
	"}");
	pass->SetVS(vs);

	PixelShader *ps = new PixelShader();
	ps->Compile("struct PixelInputType"
	"{"
		"	float4 position : SV_POSITION;"
		"	float4 color : COLOR;"
		"	float3 normal : TEXCOORD0;"
		"	float3 light : TEXCOORD1;"
	"};"
	"float4 main(PixelInputType input): SV_TARGET"
	"{"
	" float3 L = normalize(input.light); "
	"	float3 N = normalize(input.normal);"
	" float NdotL = saturate(dot(N, L)); "
	"	return float4(input.color.xyz*NdotL, input.color.a);"
	"}");
	pass->SetPS(ps);
	m->SetMaterial(pass);
	gm->SetMaterial(pass);
	while(engine.Update())
	{
		float s = engine.GetTime().spf;
		m->GetNode()->Rotate(Quaternion(s*1.0f, Vector::ONE_Y));
		gm->GetNode()->Rotate(Quaternion(s*0.8f, Vector::ONE_Y)*Quaternion(s*0.3f, Vector::ONE_X)*Quaternion(s*0.4f, Vector::ONE_Z));
		engine.Draw();
	}
	return 0;
}