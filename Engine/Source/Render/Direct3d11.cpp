#include "stdafx.h"
#include "../Math/Func.h"
#include "../Math/Color.h"
#include "../Math/ColorRGB.h"
#include "../Core/Log.h"
#include "../Material/Shader.h"
#include "../Material/Pass.h"
#include "InputLayout.h"
#include "RenderMesh.h"
#include "RenderTexture.h"
#include "Direct3d11.h"

namespace DEN
{
	RenderTarget::RenderTarget()
	{
		renderTarget = NULL;
		renderTexture = NULL;
		ZeroMemory(&renderDesc, sizeof(renderDesc));
	}

	RenderTarget::~RenderTarget()
	{
		Release();
		if (renderTexture)
			delete renderTexture;
	}

	HRESULT RenderTarget::Create(uint multisample)
	{
		HRESULT hr;
		renderTexture = new RenderTexture();
		Render::Get()->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&renderTexture->_texture);
		renderTexture->_texture->GetDesc(&renderTexture->z_desc);
		hr = Render::Get()->_device->CreateRenderTargetView(renderTexture->_texture, 0, &renderTarget);
		renderTarget->GetDesc(&renderDesc);
		return hr;
	}

	HRESULT RenderTarget::Create(RenderTexture *tex)
	{
		HRESULT hr;
		renderTexture = tex;
		//dev->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&renderTexture->_texture);
		renderTexture->_texture->GetDesc(&renderTexture->z_desc);
		renderDesc.Format = renderTexture->z_desc.Format;
		renderDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderDesc.Texture2D.MipSlice = 0;
		hr = Render::Get()->_device->CreateRenderTargetView(renderTexture->_texture, &renderDesc, &renderTarget);
		return hr;
	}

	void RenderTarget::Release()
	{
		if (renderTexture)
			renderTexture->FreeTexture();
		if(renderTarget)
			renderTarget->Release();
		renderTarget = NULL;
	}

	void RenderTarget::Restart()
	{
		HRESULT hr;
		Release();
		if(this == Render::Get()->_backTarget->render)
			Render::Get()->_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&renderTexture->_texture);
		else
			Render::Get()->_device->CreateTexture2D(&renderTexture->z_desc, NULL, &renderTexture->_texture);
		renderTexture->_texture->GetDesc(&renderTexture->z_desc);
		hr = Render::Get()->_device->CreateRenderTargetView(renderTexture->_texture, 0, &renderTarget);
		renderTexture->_texture->Release();
		//renderTexture = NULL;
	}

	void RenderTarget::Resize(uint width, uint height)
	{
		Release();
		renderTexture->z_desc.Width = width;
		renderTexture->z_desc.Height = height;
		Render::Get()->_device->CreateTexture2D(&renderTexture->z_desc, NULL, &renderTexture->_texture);
		HRESULT hr = Render::Get()->_device->CreateRenderTargetView(renderTexture->_texture, &renderDesc, &renderTarget);
	}

	DepthStencil::DepthStencil()
	{
		depthTexture = NULL;
		depthStencil = NULL;
		ZeroMemory(&depthDesc, sizeof(depthDesc));
		ZeroMemory(&depthTexDesc, sizeof(depthTexDesc));
	}

	DepthStencil::~DepthStencil()
	{
		Release();
	}

	HRESULT DepthStencil::Create(uint width, uint height, uint multisample)
	{
		HRESULT hr;
		if (width && height)
		{
			depthTexDesc.Width = width;
			depthTexDesc.Height = height;
		}
		else
		{
			depthTexDesc.Width = Render::Get()->_swapDesc.BufferDesc.Width;
			depthTexDesc.Height = Render::Get()->_swapDesc.BufferDesc.Height;
		}
		depthTexDesc.MipLevels = 1;
		depthTexDesc.ArraySize = 1;
		depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthTexDesc.SampleDesc.Count = multisample;
		depthTexDesc.SampleDesc.Quality = 0;
		depthTexDesc.Usage = D3D11_USAGE_DEFAULT;
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthTexDesc.CPUAccessFlags = 0;
		depthTexDesc.MiscFlags = 0;
		hr = Render::Get()->_device->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		if(multisample == 1)
			depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		else
			depthDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
		depthDesc.Texture2D.MipSlice = 0;
		hr = Render::Get()->_device->CreateDepthStencilView(depthTexture, &depthDesc, &depthStencil);
		return hr;
	}

	void DepthStencil::Release()
	{
		if(depthTexture)
			depthTexture->Release();
		if(depthStencil)
			depthStencil->Release();
		depthTexture = NULL;
		depthStencil = NULL;
	}

	void DepthStencil::Restart()
	{
		HRESULT hr;
		Release();
		hr = Render::Get()->_device->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		hr = Render::Get()->_device->CreateDepthStencilView(depthTexture, &depthDesc, &depthStencil);
	}

	void DepthStencil::Resize(uint width, uint height)
	{
		HRESULT hr;
		Release();
		depthTexDesc.Width = width;
		depthTexDesc.Height = height;
		hr = Render::Get()->_device->CreateTexture2D(&depthTexDesc, NULL, &depthTexture);
		hr = Render::Get()->_device->CreateDepthStencilView(depthTexture, &depthDesc, &depthStencil);
	}

	Target::Target(RenderTarget *r, DepthStencil *d)
	{
		render = r;
		depth = d;
	}

	Target::~Target()
	{
		delete render;
		delete depth;
	}
	void Target::Create(uint multisample)
	{
		render->Create(multisample);
		depth->Create(0, 0, multisample);
	}
	void Target::Create(RenderTexture *tex, uint multisample)
	{
		render->Create(tex);
		depth->Create(tex->GetTextureDesc().Width, tex->GetTextureDesc().Height, multisample);
	}

	void Target::Restart()
	{
		HRESULT hr;
		Release();
		render->Restart();

		depth->depthTexDesc.Width = render->renderTexture->GetTextureDesc().Width;
		depth->depthTexDesc.Height = render->renderTexture->GetTextureDesc().Height;
		hr = Render::Get()->_device->CreateTexture2D(&depth->depthTexDesc, NULL, &depth->depthTexture);
		hr = Render::Get()->_device->CreateDepthStencilView(depth->depthTexture, &depth->depthDesc, &depth->depthStencil);
	}

	void Target::Release()
	{
		if(render)
			render->Release();
		if(depth)
			depth->Release();
	}

	void Target::Resize(uint width, uint height)
	{
		render->Resize(width, height);
		depth->Resize(width, height);
	}

	Render *Render::_this = nullptr;
	Render::Render()
	{
		_this = this;
		_swapChain = nullptr;
		_device = nullptr;
		_deviceContext = nullptr;
		_output = nullptr;
		_defaultTarget = new Target(new RenderTarget, new DepthStencil);
		_backTarget = _defaultTarget;
		_targets.insert(_defaultTarget);
		_vSync = 0;
		_rasterState = nullptr;
		_samplerState = nullptr;
		_blendState = nullptr;
		_depthState = nullptr;
		_debug = nullptr;
	}
	Render::~Render()
	{
		for (Target* target : _targets)
			delete target;
		if(_output)
			_output->Release();
		if(_rasterState)
			_rasterState->Release();
		if(_samplerState)
			_samplerState->Release();
		if(_blendState)
			_blendState->Release();
		if(_depthState)
			_depthState->Release();
		if(_swapChain)
			_swapChain->Release();
		if(_deviceContext)
			_deviceContext->Release();
		if(_device)
			_device->Release();
	}
	/*RenderTexture *Render::CreateTexture(UINT sizeX, UINT sizeY, RESOURCE_TYPE type, DXGI_FORMAT format, UINT arraySize)
	{
		ID3D11ShaderResourceView *_res = nullptr;
		ID3D11Texture2D *_texture = nullptr;

		RenderTexture *tex = new RenderTexture;
		D3D11_TEXTURE2D_DESC desc = { 0 };
		ID3D11Device *dev = Render::Get()->_device;
		ID3D11DeviceContext *con = Render::Get()->_deviceContext;
		desc.Width = sizeX;
		desc.Height = sizeY;
		desc.MipLevels = 0U;
		desc.ArraySize = arraySize;
		desc.Format = format;//DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		if (type == RESOURCE_GPU)
		{
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			int v = min(sizeX, sizeY) / 2;
			int p = 0;
			while (v > 1)
			{
				++p;
				v /= 2;
			}
			desc.CPUAccessFlags = 0;
			desc.MipLevels = 0;
		}
		else if (type == RESOURCE_IMMUTABLE)
		{
			desc.Usage = D3D11_USAGE_IMMUTABLE;
			desc.CPUAccessFlags = 0;
		}
		else if (type == RESOURCE_DYNAMIC)
		{
			desc.MipLevels = 1;
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else if (type == RESOURCE_CPU)
		{
			desc.Usage = D3D11_USAGE_STAGING;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;
		}
		else if (type == RESOURCE_RENDER)
		{
			desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			desc.MiscFlags = 0;
			desc.CPUAccessFlags = 0;
			desc.MipLevels = 1;
		}
		else if (type == RESOURCE_DEPTH)///заменить
		{
			desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		}
		HRESULT hr;
		if (z_data == NULL)
			hr = dev->CreateTexture2D(&desc, NULL, &_texture);//CreateTexture(sizeX, sizeY, 0, D3DUSAGE_AUTOGENMIPMAP, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &tex, NULL)
		else
		{
			if (desc.Usage != D3D11_USAGE_DYNAMIC || desc.Usage != D3D11_USAGE_STAGING)
			{
				hr = dev->CreateTexture2D(&desc, NULL, &_texture);
				con->UpdateSubresource(_texture, 0, 0, z_data, sizeX * 4, sizeX*sizeY * 4);
			}
			else
			{
				desc.MipLevels = 1;
				desc.CPUAccessFlags = 0;
				D3D11_SUBRESOURCE_DATA res;
				res.pSysMem = z_data;
				res.SysMemPitch = sizeX * 4;
				res.SysMemSlicePitch = 0;
				hr = dev->CreateTexture2D(&desc, &res, &_texture);
			}
		}
		if (hr != S_OK)
			return;
		if (desc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));  //zero out shader resouce view description 
			srvDesc.Format = desc.Format;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;  //its an texture array 2d 
			srvDesc.Texture2D.MipLevels = -1;  //we want all slices, so begin at 0
			if (type == RESOURCE_RENDER)
				srvDesc.Texture2D.MipLevels = 1;
			srvDesc.Texture2D.MostDetailedMip = 0;
			hr = dev->CreateShaderResourceView(_texture, &srvDesc, &_res);
			if (hr != S_OK)
				return;
			if (type == RESOURCE_GPU)
				con->GenerateMips(_res);
		}
		_texture->GetDesc(&desc);
		return tex;
	}*/
	void Render::Resize()
	{
		if(_swapChain)
		{
			_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			for(Target* target : _targets)
				target->Release();
			DXGI_SWAP_CHAIN_DESC desc;
			_swapChain->GetDesc(&desc);
			_deviceContext->ClearState();
			_deviceContext->OMSetDepthStencilState(NULL, 0);
			_deviceContext->OMSetRenderTargets(0, NULL, NULL);
			_deviceContext->Flush();
			_debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
			_swapChain->ResizeBuffers(desc.BufferCount, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
			for (Target* target : _targets)
				target->Restart();
		}
	}
	UINT Render::GetWidth()
	{
		return _defaultTarget->render->renderTexture->GetTextureDesc().Width;
	}
	UINT Render::GetHeight()
	{
		return _defaultTarget->render->renderTexture->GetTextureDesc().Height;
	}
	Target *Render::CreateRenderTarget()
	{
		Target *ren = new Target(new RenderTarget, new DepthStencil);
		_targets.insert(ren);
		return ren;
	}
	void Render::SetVSync(bool enable)
	{
		_vSync = int(enable);
	}
	uint Render::GetMaxMSAASupport()
	{
		UINT maxQualityLevel = 0;
		for(UINT sampleCount = D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; sampleCount > 0; sampleCount /= 2)
			if(SUCCEEDED(_device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, sampleCount, &maxQualityLevel) && maxQualityLevel <= 0))
				return sampleCount;
	}
	bool Render::Initialize(HWND &hWind, UINT sizeX, UINT sizeY, bool fullscreen)
	{
		uint multiSample = 4;
		ZeroMemory(&_swapDesc, sizeof(_swapDesc));
		_swapDesc.BufferCount = 1;
		_swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		_swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		_swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		_swapDesc.BufferDesc.Width = sizeX;
		_swapDesc.BufferDesc.Height = sizeY;
		_swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		_swapDesc.OutputWindow = hWind;
		_swapDesc.SampleDesc.Count = multiSample;
		_swapDesc.SampleDesc.Quality = 0;
		_swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		_swapDesc.Windowed = !fullscreen;
		D3D_FEATURE_LEVEL features[7];
		features[0] = D3D_FEATURE_LEVEL_11_1;
		features[1] = D3D_FEATURE_LEVEL_11_0;
		features[2] = D3D_FEATURE_LEVEL_10_1;
		features[3] = D3D_FEATURE_LEVEL_10_0;
		features[4] = D3D_FEATURE_LEVEL_9_3;
		features[5] = D3D_FEATURE_LEVEL_9_2;
		features[6] = D3D_FEATURE_LEVEL_9_1;
		uint flag = D3D11_CREATE_DEVICE_SINGLETHREADED;
#ifdef _DEBUG
		flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		if(FAILED(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flag, features, 5, D3D11_SDK_VERSION, &_swapDesc, &_swapChain, &_device, &_featureLevel, &_deviceContext)))
			return false;
		_device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&_debug));
		_swapChain->GetContainingOutput(&_output);

		SetViewport(Viewport(0, 0, _swapDesc.BufferDesc.Width, _swapDesc.BufferDesc.Height, 0.0f, 1.0f));
		UINT num = 0;
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM;
		UINT flags = DXGI_ENUM_MODES_INTERLACED;
		_output->GetDisplayModeList(format, flags, &num, 0);
		DXGI_MODE_DESC * pDescs = new DXGI_MODE_DESC[num];
		_output->GetDisplayModeList(format, flags, &num, pDescs);
		for(UINT i = 0; i != num; ++i)
			_modes.push_back(DisplayMode(pDescs[i].Width, pDescs[i].Height, pDescs[i].RefreshRate.Numerator / pDescs[i].RefreshRate.Denominator, pDescs[i].Format));
		///z_display3d = DisplayMode(pMode.Width, pMode.Height, pMode.RefreshRate, pMode.Format);
		_screen = DisplayMode(sizeX, sizeY, 60, DXGI_FORMAT_R8G8B8A8_UNORM);
		_screen.Width = GetSystemMetrics(SM_CXSCREEN);
		_screen.Height = GetSystemMetrics(SM_CYSCREEN);
		_window = _screen;
		_window.Width = sizeX;
		_window.Height = sizeY;
		_defaultTarget->Create(multiSample);
		SetDefaultRenderTarget();

		_rasterDesc.AntialiasedLineEnable = false;
		_rasterDesc.CullMode = D3D11_CULL_FRONT;
		_rasterDesc.DepthBias = 0;
		_rasterDesc.DepthBiasClamp = 0.0f;
		_rasterDesc.DepthClipEnable = true;
		_rasterDesc.FillMode = D3D11_FILL_SOLID;
		_rasterDesc.FrontCounterClockwise = false;
		_rasterDesc.MultisampleEnable = true;
		_rasterDesc.ScissorEnable = false;
		_rasterDesc.SlopeScaledDepthBias = 0.0f;
		HRESULT hr = _device->CreateRasterizerState(&_rasterDesc, &_rasterState);
		_deviceContext->RSSetState(_rasterState);

		_samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		_samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		_samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		_samplerDesc.BorderColor[0] = 0.0f;
		_samplerDesc.BorderColor[1] = 0.0f;
		_samplerDesc.BorderColor[2] = 0.0f;
		_samplerDesc.BorderColor[3] = 0.0f;
		_samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		_samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		_samplerDesc.MaxAnisotropy = 16;
		_samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		_samplerDesc.MinLOD = 0.0f;
		_samplerDesc.MipLODBias = 0.0f;
		hr = _device->CreateSamplerState(&_samplerDesc, &_samplerState);
		for(int i = 0; i != 8; ++i)
			_deviceContext->PSSetSamplers(i, 1, &_samplerState);
		memset(&_blendDesc, 0, sizeof(_blendDesc));
		_blendDesc.AlphaToCoverageEnable = false;
		_blendDesc.IndependentBlendEnable = false;//если false, то только renderTarget[0] используется
		_blendDesc.RenderTarget[0].BlendEnable = true;
		_blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		_blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		_blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		_blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		_blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		_blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		_blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		hr = _device->CreateBlendState(&_blendDesc, &_blendState);
		float blendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		_deviceContext->OMSetBlendState(_blendState, blendFactor, 0xffffffff);

		_depthDesc.DepthEnable = true;
		_depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
		_depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		_depthDesc.StencilEnable = FALSE;
		_depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		_depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		_depthDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		_depthDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR;
		_depthDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		_depthDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		_depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		_depthDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		_depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		_depthDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		hr = _device->CreateDepthStencilState(&_depthDesc, &_depthState);
		_deviceContext->OMSetDepthStencilState(_depthState, 1);

		return true;
	}

	void Render::Reset()
	{
		HRESULT hr = 0;
		if(FAILED(hr))
		{
			if(hr == DXGI_STATUS_MODE_CHANGED)
				Log::Get()->Error("Device Error", "Device not be fullscreen DXGI_STATUS_MODE_CHANGED");
			else if(hr == DXGI_STATUS_MODE_CHANGED)
				Log::Get()->Error("Device Error", "Device not be fullscreen DXGI_STATUS_MODE_CHANGE_IN_PROGRESS");
			else if(hr == DXGI_ERROR_NOT_CURRENTLY_AVAILABLE)
				Log::Get()->Error("Device Error", "Device not be fullscreen DXGI_ERROR_NOT_CURRENTLY_AVAILABLE");
			else
				Log::Get()->Error("Device Error", "Device not be fullscreen changed");
			return;
		}
		hr = _swapChain->ResizeTarget(&_swapDesc.BufferDesc);
		_swapChain->GetContainingOutput(&_output);
		if(FAILED(hr))
		{
			Log::Get()->Error("Device Error", "Device not be change resolution");
			return;
		}
		_deviceContext->OMSetRenderTargets(0, NULL, NULL);
		_backTarget->Release();
		if(_swapDesc.Windowed)
			hr = _swapChain->ResizeBuffers(1, _swapDesc.BufferDesc.Width, _swapDesc.BufferDesc.Height, _swapDesc.BufferDesc.Format, 0);
		else
			hr = _swapChain->ResizeBuffers(1, _swapDesc.BufferDesc.Width, _swapDesc.BufferDesc.Height, _swapDesc.BufferDesc.Format, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
		if(FAILED(hr))
		{
			Log::Get()->Error("Device Error", "Device not be Resize Buffers");
			return;
		}
		_backTarget->Restart();
		SetDefaultRenderTarget();
		BOOL ful;
		IDXGIOutput *dev;
		_swapChain->GetFullscreenState(&ful, &dev);
		if((dev == _output && _swapDesc.Windowed) || (ful == _swapDesc.Windowed))
		{
			if(_swapDesc.Windowed)
				hr = _swapChain->SetFullscreenState(!_swapDesc.Windowed, NULL);
			else
				hr = _swapChain->SetFullscreenState(!_swapDesc.Windowed, _output);
		}
	}

	void Render::SetViewport(const Viewport &port)
	{
		_view= port;
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)port.GetSizeX();
		vp.Height = (FLOAT)port.GetSizeY();
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = (float)port.GetX();
		vp.TopLeftY = (float)port.GetY();
		_deviceContext->RSSetViewports(1, &vp);
	}

	Viewport Render::GetViewport()
	{
		D3D11_VIEWPORT vp;
		UINT num = 1;
		_deviceContext->RSGetViewports(&num, &vp);
		Viewport port;
		port.SetPoint(toInt(vp.TopLeftX), toInt(vp.TopLeftY));
		port.SetSize(toInt(vp.Width), toInt(vp.Height));
		port.SetClip(vp.MinDepth, vp.MaxDepth);
		return port;
	}
	void Render::SetTarget(Target *target)
	{
		if(target == NULL)
			target = _backTarget;
		SetViewport(Viewport(0, 0, target->render->renderTexture->GetTextureDesc().Width, target->render->renderTexture->GetTextureDesc().Height, 0.0f, 1.0f));
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		_deviceContext->PSSetShaderResources(0, 1, pSRV);
		_deviceContext->OMSetRenderTargets(1, &target->render->renderTarget, target->depth->depthStencil);
	}
	void Render::SetRenderTarget(Target *target)
	{
		_defaultTarget = target;
		if(!_defaultTarget)
			_defaultTarget = _backTarget;
		SetViewport(Viewport(0, 0, _defaultTarget->render->renderTexture->GetTextureDesc().Width, _defaultTarget->render->renderTexture->GetTextureDesc().Height, 0.0f, 1.0f));
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		_deviceContext->PSSetShaderResources(0, 1, pSRV);
		_deviceContext->OMSetRenderTargets(1, &_defaultTarget->render->renderTarget, _defaultTarget->depth->depthStencil);
	}
	void Render::SetDefaultRenderTarget()
	{
		SetViewport(Viewport(0, 0, _defaultTarget->render->renderTexture->GetTextureDesc().Width, _defaultTarget->render->renderTexture->GetTextureDesc().Height, 0.0f, 1.0f));
		ID3D11ShaderResourceView *const pSRV[1] = { NULL };
		_deviceContext->PSSetShaderResources(0, 1, pSRV);
		_deviceContext->OMSetRenderTargets(1, &_defaultTarget->render->renderTarget, _defaultTarget->depth->depthStencil);
	}
	void Render::DeleteRenderTarget(Target *target)
	{
		auto r = _targets.find(target);
		if(r == _targets.end())
			return;
		delete (*r);
		_targets.erase(r);
	}

	void Render::ClearRenderTarget(RenderTarget *target, const Color &color)
	{
		if(target)
			_deviceContext->ClearRenderTargetView(target->renderTarget, &color.r);
	}

	void Render::ClearDepthStencil(DepthStencil *depth, float value)
	{
		if(depth)
			_deviceContext->ClearDepthStencilView(depth->depthStencil, D3D11_CLEAR_DEPTH, value, 0);
	}

	void Render::ClearDepthStencil(DepthStencil *depth, UCHAR stencil)
	{
		if(depth)
			_deviceContext->ClearDepthStencilView(depth->depthStencil, D3D11_CLEAR_STENCIL, 1.0f, stencil);
	}

	void Render::ClearDepthStencil(DepthStencil *depth, float value, UCHAR stencil)
	{
		if(depth)
			_deviceContext->ClearDepthStencilView(depth->depthStencil, D3D11_CLEAR_DEPTH + D3D11_CLEAR_STENCIL, value, stencil);
	}

	void Render::Begin(const Color &clear, float depth)
	{
		ClearRenderTarget(_defaultTarget->render, clear);
		ClearDepthStencil(_defaultTarget->depth, depth, 0);
	}

	void Render::End()
	{
		_swapChain->Present(_vSync, 0);// Present the information rendered to the back buffer to the front buffer (the screen)
	}
	void Render::RenderPass(Pass *pass)
	{
		if(pass == NULL)
			return;
		DWORD val2;
		val2 = pass->GetLightEnable();
		val2 = pass->GetAlphaFunc();
		val2 = pass->GetAlphaBlend();
		val2 = pass->GetAlphaTest();
		_depthDesc.DepthEnable = pass->GetDepthEnable();
		_rasterDesc.DepthClipEnable = pass->GetDepthWrite();
		_depthDesc.DepthFunc = (D3D11_COMPARISON_FUNC)pass->GetDepthFunc();
		if(pass->GetDepthWrite())
			_depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			_depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		_rasterDesc.CullMode = (D3D11_CULL_MODE)pass->GetCull();
		_blendDesc.RenderTarget[0].SrcBlend = (D3D11_BLEND)pass->GetSrcBlend();
		_blendDesc.RenderTarget[0].DestBlend = (D3D11_BLEND)pass->GetDestBlend();
		_blendDesc.RenderTarget[0].SrcBlendAlpha = (D3D11_BLEND)pass->GetSrcAlpha();
		_blendDesc.RenderTarget[0].DestBlendAlpha = (D3D11_BLEND)pass->GetDestAlpha();
		_blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		_blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

		_blendState->Release();
		_depthState->Release();
		_rasterState->Release();
		_device->CreateBlendState(&_blendDesc, &_blendState);
		_device->CreateRasterizerState(&_rasterDesc, &_rasterState);
		_device->CreateDepthStencilState(&_depthDesc, &_depthState);
		float blendFactor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
		_deviceContext->OMSetBlendState(_blendState, blendFactor, 0xffffffff);
		_deviceContext->RSSetState(_rasterState);
		_deviceContext->OMSetDepthStencilState(_depthState, 1);
		VertexShader *sh = pass->GetVS();
		if(sh == NULL)
			_deviceContext->VSSetShader(NULL, NULL, 0);
		else
			_deviceContext->VSSetShader((ID3D11VertexShader*)sh->GetBuffer(), NULL, 0);
		PixelShader *ps = pass->GetPS();
		if(ps == NULL)
			_deviceContext->PSSetShader(NULL, NULL, 0);
		else
			_deviceContext->PSSetShader((ID3D11PixelShader*)ps->GetBuffer(), NULL, 0);
		GeometryShader *gs = pass->GetGS();
		if(gs == NULL)
			_deviceContext->GSSetShader(NULL, NULL, 0);
		else
			_deviceContext->GSSetShader((ID3D11GeometryShader*)gs->GetBuffer(), NULL, 0);
		HullShader *hs = pass->GetHS();
		if(hs == NULL)
			_deviceContext->HSSetShader(NULL, NULL, 0);
		else
			_deviceContext->HSSetShader((ID3D11HullShader*)hs->GetBuffer(), NULL, 0);
		DomainShader *ds = pass->GetDS();
		if(ds == NULL)
			_deviceContext->DSSetShader(NULL, NULL, 0);
		else
			_deviceContext->DSSetShader((ID3D11DomainShader*)ds->GetBuffer(), NULL, 0);
		pass->RenderTextures();
	}
	void Render::ExecuteMesh(RenderMesh *mesh)
	{
		_deviceContext->IASetInputLayout((ID3D11InputLayout*)mesh->GetInputLayout()->GetData());
		mesh->Update();
		if(mesh->IsIndex())
			_deviceContext->DrawIndexed(mesh->GetIndexCount(), 0, 0);
		else
			_deviceContext->Draw(mesh->GetVertexCount(), 0);
	}
	void Render::SetWindow(UINT windowX, UINT windowY, UINT renderX, UINT renderY, bool maxSize, HWND hWindow)
	{
		_window.Width = windowX;
		_window.Height = windowY;
		_swapDesc.Windowed = TRUE;
		_swapDesc.BufferDesc.Width = renderX;
		_swapDesc.BufferDesc.Height = renderY;
		_swapDesc.BufferDesc.RefreshRate.Numerator = 60;
		_swapDesc.BufferDesc.RefreshRate.Denominator = 1;
		HWND w = GetParent(hWindow);
		WINDOWINFO info, info1;
		GetWindowInfo(w, &info1);
		GetWindowInfo(hWindow, &info);
		if(w)
		{
			GetWindowInfo(w, &info1);
			info.rcClient.left = info.rcClient.left - info1.rcClient.left;
			info.rcClient.top = info.rcClient.top - info1.rcClient.top;
		}
		else
			info.dwStyle |= WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_POPUP;
		int posX = info.rcClient.left;
		int posY = info.rcClient.top;
		RECT r;
		r.left = 0;
		r.top = 0;
		r.right = 0;
		r.bottom = 0;
		if(maxSize)
			info.dwStyle |= WS_THICKFRAME | WS_MAXIMIZE | WS_MAXIMIZEBOX;
		AdjustWindowRect(&r, info.dwStyle, false);
		int sizeX = _window.Width;
		int sizeY = _window.Height;
		sizeX += (r.right - r.left);
		sizeY += (r.bottom - r.top);
		SetWindowLong(_swapDesc.OutputWindow, GWL_STYLE, info.dwStyle);
		SetWindowPos(hWindow, NULL, posX, posY, sizeX, sizeY, SWP_SHOWWINDOW | SWP_FRAMECHANGED);
	}
	bool Render::CompileShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines, ID3DBlob **bytes)
	{
		D3D_SHADER_MACRO macro = {"", ""};
		UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
#ifdef _DEBUG
		compileFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob = NULL;
		HRESULT result = D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), shaderModel.c_str(), compileFlags, 0, bytes, &pErrorBlob);
		if(FAILED(result))
		{
			if(pErrorBlob != NULL)
				OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}

		if(pErrorBlob != NULL)
			pErrorBlob->Release();
		if(FAILED(result))
			return false;
		return true;
	}
	bool Render::CreateVertexShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines)
	{
		ID3DBlob* pBytecodeBlob = NULL;

		HRESULT result = CompileShader(filename, entryPoint, shaderModel, defines, &pBytecodeBlob);
		if(FAILED(result))
			return false;
		ID3D11VertexShader *mpShader = nullptr;
		result = _device->CreateVertexShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), NULL, &mpShader);
		pBytecodeBlob->Release();

		if(FAILED(result))
			return false;
		return true;
	}
	bool Render::CreatePixelShader(const wstring &filename, const string &entryPoint, const string &shaderModel, const string &defines)
	{
		ID3DBlob* pBytecodeBlob = NULL;

		HRESULT result = CompileShader(filename, entryPoint, shaderModel, defines, &pBytecodeBlob);
		if(FAILED(result))
			return false;
		ID3D11PixelShader *mpShader = nullptr;
		result = _device->CreatePixelShader(pBytecodeBlob->GetBufferPointer(), pBytecodeBlob->GetBufferSize(), NULL, &mpShader);
		pBytecodeBlob->Release();

		if(FAILED(result))
			return false;
		return true;
	}
};