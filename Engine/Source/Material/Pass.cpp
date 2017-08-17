#include "stdafx.h"
#include "Shader.h"
#include "Texture.h"
#include "Pass.h"
namespace DEN
{
	Pass::Pass()
	{
		_vs = nullptr;
		_ps = nullptr;
		_gs = nullptr;
		_hs = nullptr;
		_ds = nullptr;
		_lightning = true;
		_alphaBlend = false;
		_alphaTest = false;
		_enable = true;
		_cull = D3DCULL_CCW;
		_srcblend = D3DBLEND_ONE;
		_destblend = D3DBLEND_ZERO;
		_srcAblend = D3DBLEND_ONE;
		_destAblend = D3DBLEND_ONE;
		_depthFunc = D3DCMP_LESS;
		_alphaFunc = D3DCMP_GREATEREQUAL;
		_zwrite = true;
		_zenable = true;
	}

	Pass::~Pass()
	{
		
	}
	void Pass::SetTexture(UCHAR slot, Texture *tex)
	{
		if(tex)
			_textures[slot] = tex;
		else
			_textures.erase(slot);
	}
	Texture *Pass::GetTexture(UCHAR slot)
	{
		auto f = _textures.find(slot);
		if(f == _textures.end())
			return nullptr;
		return f->second;
	}
	void Pass::SetVS(VertexShader *shader)
	{
		_vs = shader;
	}
	VertexShader *Pass::GetVS()
	{
		return _vs;
	}
	void Pass::SetPS(PixelShader *shader)
	{
		_ps = shader;
	}
	PixelShader *Pass::GetPS()
	{
		return _ps;
	}
	void Pass::SetGS(GeometryShader *shader)
	{
		_gs = shader;
	}
	GeometryShader *Pass::GetGS()
	{
		return _gs;
	}
	void Pass::SetHS(HullShader *shader)
	{
		_hs = shader;
	}
	HullShader *Pass::GetHS()
	{
		return _hs;
	}
	void Pass::SetDS(DomainShader *shader)
	{
		_ds = shader;
	}
	DomainShader *Pass::GetDS()
	{
		return _ds;
	}
	void Pass::SetDepthFunc(D3DCMPFUNC func)
	{
		_depthFunc = func;
	}

	void Pass::SetAlphaFunc(D3DCMPFUNC func)
	{
		_alphaFunc = func;
	}

	void Pass::SetAlphaBlend(bool enable)
	{
		_alphaBlend = enable;
	}

	void Pass::SetAlphaTest(bool enable)
	{
		_alphaTest = enable;
	}

	void Pass::SetEnable(bool enable)
	{
		_enable = enable;
	}

	bool Pass::IsEnable()
	{
		return _enable;
	}

	bool Pass::GetAlphaTest()
	{
		return _alphaTest;
	}

	bool Pass::GetAlphaBlend()
	{
		return _alphaBlend;
	}

	DWORD Pass::GetSrcBlend()
	{
		return _srcblend;
	}

	DWORD Pass::GetDestBlend()
	{
		return _destblend;
	}

	DWORD Pass::GetSrcAlpha()
	{
		return _srcAblend;
	}

	DWORD Pass::GetDestAlpha()
	{
		return _destAblend;
	}

	DWORD Pass::GetDepthFunc()
	{
		return _depthFunc;
	}

	DWORD Pass::GetAlphaFunc()
	{
		return _alphaFunc;
	}

	void Pass::SetLightEnable(bool enable)
	{
		_lightning = enable;
	}

	void Pass::SetBlend(D3DBLEND srcblend, D3DBLEND destblend)
	{
		_srcblend = srcblend;
		_destblend = destblend;
	}

	void Pass::SetAlphaBlend(D3DBLEND srcblend, D3DBLEND destblend)
	{
		_srcAblend = srcblend;
		_destAblend = destblend;
	}

	bool Pass::GetLightEnable()
	{
		return _lightning;
	}

	void Pass::SetCull(D3DCULL cull)
	{
		_cull = cull;
	}

	void Pass::SetDepthWrite(bool enable)
	{
		_zwrite = enable;
	}

	void Pass::SetDepthEnable(bool enable)
	{
		_zenable = enable;
	}

	D3DCULL Pass::GetCull()
	{
		return _cull;
	}

	bool Pass::GetDepthWrite()
	{
		return _zwrite;
	}

	bool Pass::GetDepthEnable()
	{
		return _zenable;
	}
	void Pass::RenderTextures()
	{
		for(auto &p: _textures)
			p.second->Render(p.first);
	}
}