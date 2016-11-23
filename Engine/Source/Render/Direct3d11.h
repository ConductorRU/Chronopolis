#pragma once
namespace DEN
{
	class Color;
	class ColorRGB;
	class RenderMesh;
	class ConstantBuffer;
	class Pass;
	class Shader;
	class VertexShader;
	class PixelShader;
	class Viewport//: public D3DVIEWPORT9
	{
	private:
		DWORD       X;
		DWORD       Y;            /* Viewport Top left */
		DWORD       Width;
		DWORD       Height;       /* Viewport Dimensions */
		float       MinZ;         /* Min/max of clip Volume */
		float       MaxZ;
	public:
		Viewport(UINT x = 0, UINT y = 0, UINT sizeX = 100, UINT sizeY = 100, float zMin = 0.0f, float zMax = 1.0f)
		{
			X = x;
			Y = y;
			Width = sizeX;
			Height = sizeY;
			MinZ = zMin;
			MaxZ = zMax;
		}
		inline void Set(UINT x, UINT y, UINT sizeX, UINT sizeY) { X = x; Y = y; Width = sizeX; Height = sizeY; };
		inline void Set(UINT x, UINT y, UINT sizeX, UINT sizeY, float minDepth, float maxDepth) { X = x; Y = y; Width = sizeX; Height = sizeY; MinZ = minDepth; MaxZ = maxDepth; };
		inline void SetPoint(UINT x, UINT y) { X = x; Y = y; };
		inline void SetSize(UINT sizeX, UINT sizeY) { Width = sizeX; Height = sizeY; };
		inline void SetClip(float zMin = 0.0f, float zMax = 1.0f) { MinZ = zMin; MaxZ = zMax; };
		inline UINT GetSizeX() const { return Width; };
		inline UINT GetSizeY() const { return Height; };
		inline UINT GetX() const { return X; };
		inline UINT GetY() const { return Y; };
		inline float GetMinClip() const { return MinZ; };
		inline float GetMaxClip() const { return MaxZ; };
	};
	struct DisplayMode
	{
		UINT Width;
		UINT Height;
		UINT RefreshRate;
		DWORD Format;
		DisplayMode() {};
		DisplayMode(UINT w, UINT h, UINT r, DWORD f)
		{
			Width = w;
			Height = h;
			RefreshRate = r;
			Format = f;
		}
	};
	struct DepthStencil
	{
		friend class Render;
		friend struct Target;
	private:
		ID3D11DepthStencilView* depthStencil;
		ID3D11Texture2D* depthTexture;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthDesc;
		D3D11_TEXTURE2D_DESC depthTexDesc;
	public:
		DepthStencil();
		~DepthStencil();
		HRESULT Create(Render *dev);
		void Restart(Render *dev);
		void Release();
	};
	struct RenderTarget
	{
		friend class Render;
		friend struct Target;
	private:
		ID3D11RenderTargetView* renderTarget;
		ID3D11Texture2D* renderTexture;
		D3D11_RENDER_TARGET_VIEW_DESC renderDesc;
		D3D11_TEXTURE2D_DESC renderTexDesc;
	public:
		RenderTarget();
		~RenderTarget();
		HRESULT Create(Render *dev);
		void Restart(Render *dev);
		void Release();
	};

	struct Target
	{
		RenderTarget *render;
		DepthStencil *depth;
		Target(RenderTarget *r, DepthStencil *d);
		~Target();
		void Restart(Render *dev);
		void Release();
	};
	class Render
	{
		friend DepthStencil;
		friend RenderTarget;
		friend Target;
		friend RenderMesh;
		friend ConstantBuffer;
		friend Shader;
		friend VertexShader;
		friend PixelShader;
		friend InputLayout;
	private:
		static Render *_this;
		DXGI_SWAP_CHAIN_DESC _swapDesc;
		D3D_FEATURE_LEVEL _featureLevel;
		vector<DisplayMode> _modes;
		IDXGISwapChain *_swapChain;
		ID3D11Device *_device; // Our rendering device
		ID3D11DeviceContext *_deviceContext;
		IDXGIOutput *_output;
		DisplayMode _screen;//характеристики экрана
		DisplayMode _window;//характеристики окна
		Viewport _view;
		Target *_defaultTarget;
		Target *_backTarget;
		set<Target*> _targets;
		int _vSync;
		ID3D11RasterizerState *_rasterState;
		ID3D11SamplerState *_samplerState;
		ID3D11BlendState *_blendState;
		ID3D11DepthStencilState *_depthState;
		D3D11_RASTERIZER_DESC _rasterDesc;
		D3D11_SAMPLER_DESC _samplerDesc;
		D3D11_BLEND_DESC _blendDesc;
		D3D11_DEPTH_STENCIL_DESC _depthDesc;
		bool CompileShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines, ID3DBlob **bytes);
	public:
		static Render *Get() { return _this; };
		UINT GetWidth() { return _window.Width; };
		UINT GetHeight() { return _window.Height; };
		float GetAspect() { return (float)_window.Width / (float)_window.Height; };
		Render();
		~Render();
		void SetVSync(bool enable);
		bool Initialize(HWND &hWind, UINT sizeX, UINT sizeY, bool fullscreen);
		void Reset();
		void SetViewport(const Viewport &port);
		Viewport GetViewport();
		void SetTarget(Target *target);
		void SetRenderTarget(Target *target);
		void SetDefaultRenderTarget();
		void DeleteRenderTarget(Target *target);
		void ClearRenderTarget(RenderTarget *target, const Color &color);
		void ClearDepthStencil(DepthStencil *depth, float value);
		void ClearDepthStencil(DepthStencil *depth, UCHAR stencil);
		void ClearDepthStencil(DepthStencil *depth, float value, UCHAR stencil);
		void Begin(const Color &clear, float depth = 1.0f);
		void End();
		void RenderPass(Pass *pass);
		void ExecuteMesh(RenderMesh *mesh);
		void SetWindow(UINT windowX, UINT windowY, UINT renderX, UINT renderY, bool maxSize, HWND hWindow);
		bool CreateVertexShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines);
		bool CreatePixelShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines);
	};
}