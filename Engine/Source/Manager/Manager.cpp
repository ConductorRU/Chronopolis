#include "stdafx.h"
#include "../Mesh/Mesh.h"
#include "../Mesh/Paramesh.h"
#include "../Material/Shader.h"
#include "../Material/Pass.h"
#include "../GUI/Font.h"
#include "../Render/InputLayout.h"
#include "../Format/Class.h"
#include "../Material/Texture.h"
#include "Manager.h"
namespace DEN
{
	Manager *Manager::_this = nullptr;
	Manager::Manager()
	{
		_this = this;
		InputLayout *ia = CreateInputLayout("default");
		ia->Add("POSITION", DXGI_FORMAT_R32G32B32_FLOAT);
		ia->Add("COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT);
		ia->Add("NORMAL", DXGI_FORMAT_R32G32B32_FLOAT);
		ia->Add("UV", DXGI_FORMAT_R32G32_FLOAT);
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
	string Manager::GetExtension(const string &filename)
	{
		size_t size = filename.size();
		string res = "";
		for(size_t s = size - 1u; s != 0; --s)
		{
			if(filename[s] == '.')
				return res;
			res = filename[s] + res;
		}
		return "";
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
	InputLayout *Manager::CreateInputLayout(const string &defaultName)
	{
		InputLayout *in = new InputLayout();
		_inputs.insert(in);
		if(defaultName != "")
			_inputDefault[defaultName] = in;
		return in;
	}
	InputLayout *Manager::GetInputLayout(const string &defaultName)
	{
		map<string, InputLayout*>::const_iterator iter = _inputDefault.find(defaultName);
		if(iter != _inputDefault.end())
			return iter->second;
		return nullptr;
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
	Texture *Manager::LoadTexture(const string &filename, bool isSprite)
	{
		string ext = GetExtension(filename);
		transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		ImageFormat *img = nullptr;
		if(ext == "tga")
			img = TGA::Load(filename.c_str());
		else if(ext == "bmp")
			img = BMP::Load(filename.c_str());
		if(img)
		{
			uchar *data = img->GetData();
			Texture *bTex = new Texture();
			bTex->SetAddress(TEXTURE_ADDRESS_WRAP);
			if(isSprite)
				bTex->Create(img->GetWidth(), img->GetHeight(), RESOURCE_SPRITE);
			else
				bTex->Create(img->GetWidth(), img->GetHeight(), RESOURCE_GPU);
			bTex->SetRaw(img->GetData(), img->GetWidth(), img->GetHeight());
			delete img;
			_texture.insert(bTex);
			return bTex;
		}
		return nullptr;
	}
}