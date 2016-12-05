#pragma once
#include "../Math/Color.h"
#include "../Render/RenderTexture.h"
namespace DEN
{
	enum TEXTURE_ADDRESS
	{
		TEXTURE_ADDRESS_WRAP = 1,
		TEXTURE_ADDRESS_MIRROR = 2,
		TEXTURE_ADDRESS_CLAMP = 3,
		TEXTURE_ADDRESS_BORDER = 4,
		TEXTURE_ADDRESS_MIRROR_ONCE = 5
	};
	enum COMPARISON_FUNC
	{
		COMPARISON_NEVER = 1,
		COMPARISON_LESS = 2,
		COMPARISON_EQUAL = 3,
		COMPARISON_LESS_EQUAL = 4,
		COMPARISON_GREATER = 5,
		COMPARISON_NOT_EQUAL = 6,
		COMPARISON_GREATER_EQUAL = 7,
		COMPARISON_ALWAYS = 8
	};
	enum TEXTURE_FILTER
	{
		TEXTURE_FILTER_MIN_MAG_MIP_POINT = 0,
		TEXTURE_FILTER_MIN_MAG_POINT_MIP_LINEAR = 0x1,
		TEXTURE_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x4,
		TEXTURE_FILTER_MIN_POINT_MAG_MIP_LINEAR = 0x5,
		TEXTURE_FILTER_MIN_LINEAR_MAG_MIP_POINT = 0x10,
		TEXTURE_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x11,
		TEXTURE_FILTER_MIN_MAG_LINEAR_MIP_POINT = 0x14,
		TEXTURE_FILTER_MIN_MAG_MIP_LINEAR = 0x15,
		TEXTURE_FILTER_ANISOTROPIC = 0x55,
		TEXTURE_FILTER_COMPARISON_MIN_MAG_MIP_POINT = 0x80,
		TEXTURE_FILTER_COMPARISON_MIN_MAG_POINT_MIP_LINEAR = 0x81,
		TEXTURE_FILTER_COMPARISON_MIN_POINT_MAG_LINEAR_MIP_POINT = 0x84,
		TEXTURE_FILTER_COMPARISON_MIN_POINT_MAG_MIP_LINEAR = 0x85,
		TEXTURE_FILTER_COMPARISON_MIN_LINEAR_MAG_MIP_POINT = 0x90,
		TEXTURE_FILTER_COMPARISON_MIN_LINEAR_MAG_POINT_MIP_LINEAR = 0x91,
		TEXTURE_FILTER_COMPARISON_MIN_MAG_LINEAR_MIP_POINT = 0x94,
		TEXTURE_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR = 0x95,
		TEXTURE_FILTER_COMPARISON_ANISOTROPIC = 0xd5
	};
	class Texture: public RenderTexture
	{
	private:
		D3D11_SAMPLER_DESC _desc;
		ID3D11SamplerState *_state;
		bool _update;
	public:
		TEXTURE_ADDRESS GetAddressU() {return (TEXTURE_ADDRESS)_desc.AddressU;};
		TEXTURE_ADDRESS GetAddressV() { return (TEXTURE_ADDRESS)_desc.AddressV; };
		TEXTURE_ADDRESS GetAddressW() { return (TEXTURE_ADDRESS)_desc.AddressW; };
		Color &GetBorderColor() { return *((Color*)_desc.BorderColor); };
		COMPARISON_FUNC GetComparsion() { return (COMPARISON_FUNC)_desc.ComparisonFunc; };
		TEXTURE_FILTER GetFilter() { return (TEXTURE_FILTER)_desc.Filter; };
		UINT GetMaxAnisotropy() { return _desc.MaxAnisotropy; };
		float GetMinLOD() { return _desc.MinLOD; };
		float GetMaxLOD() { return _desc.MaxLOD; };
		float GetMipLODBias() { return _desc.MipLODBias; };

		void SetAddressU(TEXTURE_ADDRESS val) { _desc.AddressU = (D3D11_TEXTURE_ADDRESS_MODE)val; _update = true; };
		void SetAddressV(TEXTURE_ADDRESS val) { _desc.AddressV = (D3D11_TEXTURE_ADDRESS_MODE)val; _update = true; };
		void SetAddressW(TEXTURE_ADDRESS val) { _desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)val; _update = true; };
		void SetAddress(TEXTURE_ADDRESS val) { _desc.AddressU = _desc.AddressV = _desc.AddressW = (D3D11_TEXTURE_ADDRESS_MODE)val; _update = true; };
		void SetBorderColor(Color val) { memcpy(_desc.BorderColor, &val, sizeof(float)*4); _update = true; };
		void SetComparsion(COMPARISON_FUNC val) { _desc.ComparisonFunc = (D3D11_COMPARISON_FUNC)val; _update = true; };
		void SetFilter(TEXTURE_FILTER val) { _desc.Filter = (D3D11_FILTER)val; _update = true; };
		void SetMaxAnisotropy(UINT val) { _desc.MaxAnisotropy = val; _update = true; };
		void SetMinLOD(float val) { _desc.MinLOD = val; _update = true; };
		void SetMaxLOD(float val) { _desc.MaxLOD = val; _update = true; };
		void SetMipLODBias(float val) { _desc.MipLODBias = val; _update = true; };
		Texture();
		~Texture();
		void Render(uint slot);
	};
}