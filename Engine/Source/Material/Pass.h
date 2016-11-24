#pragma once
namespace DEN
{
	typedef enum _D3DBLEND {
		D3DBLEND_ZERO = 1,
		D3DBLEND_ONE = 2,
		D3DBLEND_SRCCOLOR = 3,
		D3DBLEND_INVSRCCOLOR = 4,
		D3DBLEND_SRCALPHA = 5,
		D3DBLEND_INVSRCALPHA = 6,
		D3DBLEND_DESTALPHA = 7,
		D3DBLEND_INVDESTALPHA = 8,
		D3DBLEND_DESTCOLOR = 9,
		D3DBLEND_INVDESTCOLOR = 10,
		D3DBLEND_SRCALPHASAT = 11,
		D3DBLEND_BOTHSRCALPHA = 12,
		D3DBLEND_BOTHINVSRCALPHA = 13,
		D3DBLEND_BLENDFACTOR = 14, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
		D3DBLEND_INVBLENDFACTOR = 15, /* Only supported if D3DPBLENDCAPS_BLENDFACTOR is on */
		D3DBLEND_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
	} D3DBLEND;
	typedef enum _D3DCULL {
		D3DCULL_NONE = 1,
		D3DCULL_CW = 2,
		D3DCULL_CCW = 3,
		D3DCULL_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
	} D3DCULL;

	typedef enum _D3DCMPFUNC {
		D3DCMP_NEVER = 1,
		D3DCMP_LESS = 2,
		D3DCMP_EQUAL = 3,
		D3DCMP_LESSEQUAL = 4,
		D3DCMP_GREATER = 5,
		D3DCMP_NOTEQUAL = 6,
		D3DCMP_GREATEREQUAL = 7,
		D3DCMP_ALWAYS = 8,
		D3DCMP_FORCE_DWORD = 0x7fffffff, /* force 32-bit size enum */
	} D3DCMPFUNC;
	class Pass
	{
	protected:
		VertexShader *_vs;
		PixelShader *_ps;
		GeometryShader *_gs;
		HullShader *_hs;
		DomainShader *_ds;
		bool _lightning;
		bool _zwrite;
		bool _zenable;
		bool _alphaBlend;
		bool _alphaTest;
		bool _enable;
		D3DCULL _cull;
		D3DBLEND _srcblend;
		D3DBLEND _destblend;
		D3DBLEND _srcAblend;
		D3DBLEND _destAblend;
		D3DCMPFUNC _depthFunc;
		D3DCMPFUNC _alphaFunc;
	public:
		Pass();
		~Pass();
		void SetVS(VertexShader *shader);
		VertexShader *GetVS();
		void SetPS(PixelShader *shader);
		PixelShader *GetPS();
		void SetGS(GeometryShader *shader);
		GeometryShader *GetGS();
		void SetHS(HullShader *shader);
		HullShader *GetHS();
		void SetDS(DomainShader *shader);
		DomainShader *GetDS();
		void SetLightEnable(bool enable);
		void SetBlend(D3DBLEND srcblend, D3DBLEND destblend);
		void SetAlphaBlend(D3DBLEND srcblend, D3DBLEND destblend);
		bool GetLightEnable();
		void SetCull(D3DCULL cull);
		void SetDepthWrite(bool enable);//пишет данные в Z-буффер
		void SetDepthEnable(bool enable);//если false -выводит пиксели без чтения из Z-буффера
		void SetDepthFunc(D3DCMPFUNC func);
		void SetAlphaFunc(D3DCMPFUNC func);
		void SetAlphaBlend(bool enable);
		void SetAlphaTest(bool enable);//пиксели, не прошедшие альфа-тест, не будут отрисовываться (может пригодиться для травы, листвы, непрозрачных объектов с альфа-маской)
		void SetEnable(bool enable);//отключить проход
		bool IsEnable();
		bool GetAlphaTest();
		bool GetAlphaBlend();
		DWORD GetSrcBlend();
		DWORD GetDestBlend();
		DWORD GetSrcAlpha();
		DWORD GetDestAlpha();
		DWORD GetDepthFunc();
		DWORD GetAlphaFunc();
		D3DCULL GetCull();
		bool GetDepthWrite();
		bool GetDepthEnable();
	};
}