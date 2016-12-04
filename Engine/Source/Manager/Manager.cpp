#include "stdafx.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Paramesh.h"
#include "../Material/Shader.h"
#include "../Material/Pass.h"
#include "../GUI/Font.h"
#include "../Render/InputLayout.h"
#include "Manager.h"
namespace DEN
{
	Manager *Manager::_this = nullptr;
	Manager::Manager()
	{
		_this = this;
	}
	Manager::~Manager()
	{
		for(Mesh *m : _meshes)
			delete m;
		for(Paramesh *m : _paras)
			delete m;
		for(InputLayout *m : _inputs)
			delete m;
		for(Pass *m : _passes)
			delete m;
		for(VertexShader *vs : _vs)
			delete vs;
		for(PixelShader *ps : _ps)
			delete ps;
		for(GeometryShader *gs : _gs)
			delete gs;
		for(HullShader *hs : _hs)
			delete hs;
		for(DomainShader *ds : _ds)
			delete ds;
		for(Font *f : _fonts)
			delete f;
		_this = nullptr;
	}
	Mesh *Manager::CreateMesh(InputLayout *ia)
	{
		Mesh *mesh = new Mesh(ia);
		_meshes.insert(mesh);
		return mesh;
	}
	Paramesh *Manager::CreateParamesh()
	{
		Paramesh *mesh = new Paramesh();
		_paras.insert(mesh);
		return mesh;
	}
	InputLayout *Manager::CreateInputLayout()
	{
		InputLayout *in = new InputLayout();
		_inputs.insert(in);
		return in;
	}
	Pass *Manager::CreatePass()
	{
		Pass *pass = new Pass();
		_passes.insert(pass);
		return pass;
	}
	VertexShader *Manager::CreateVS()
	{
		VertexShader *sh = new VertexShader();
		_vs.insert(sh);
		return sh;
	}
	PixelShader *Manager::CreatePS()
	{
		PixelShader *sh = new PixelShader();
		_ps.insert(sh);
		return sh;
	}
	GeometryShader *Manager::CreateGS()
	{
		GeometryShader *sh = new GeometryShader();
		_gs.insert(sh);
		return sh;
	}
	HullShader *Manager::CreateHS()
	{
		HullShader *sh = new HullShader();
		_hs.insert(sh);
		return sh;
	}
	DomainShader *Manager::CreateDS()
	{
		DomainShader *sh = new DomainShader();
		_ds.insert(sh);
		return sh;
	}
	Font *Manager::LoadFont(const string &family, int height, bool isBold, bool isItalic)
	{
		Font *f = new Font();
		f->LoadFont(family, height, isBold, isItalic);
		_fonts.insert(f);
		return f;
	}
}