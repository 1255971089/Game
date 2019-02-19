
#include "CGameStub.h"
#include"d3dUtil.h"
#include"CTimer.h"

_XE_BEGIN

CGameStub::CGameStub(HINSTANCE hinstance):
	m_hinstance(hinstance),
	m_mainwndcaption(L"D3D11 Application"),
	m_d3ddrivertype(D3D_DRIVER_TYPE_HARDWARE),
	m_clientwidth(800),
	m_clientheight(600),
	m_enable4xMsaa(false),
	m_mainhwnd(NULL),
	m_paused(false),
	m_minimized(false),
	m_maximized(false),
	m_resizing(false),
	m_4xMsaaQuality(0),
	m_timer(new CTimer()),
	m_d3ddevice(NULL),
	m_d3dimmediatecontext(NULL),
	m_swapchain(NULL),
	m_depthstencilbuffer(NULL),
	m_rendertargetview(NULL),
	m_depthstencilview(NULL)
{
	ZeroMemory(&m_screenviewport, sizeof(D3D11_VIEWPORT));
}

CGameStub::~CGameStub()
{
	ReleaseCOM(m_rendertargetview);
	ReleaseCOM(m_depthstencilview);
	ReleaseCOM(m_swapchain);
	ReleaseCOM(m_depthstencilbuffer);

	if (m_d3dimmediatecontext)
		m_d3dimmediatecontext->ClearState();

	ReleaseCOM(m_d3dimmediatecontext);
	ReleaseCOM(m_d3ddevice);
}

void CGameStub::OnResize()
{
	assert(m_d3dimmediatecontext);
	assert(m_d3ddevice);
	assert(m_swapchain);

	ReleaseCOM(m_rendertargetview);
	ReleaseCOM(m_depthstencilview);
	ReleaseCOM(m_depthstencilbuffer);

	HR(m_swapchain->ResizeBuffers(1,
		m_clientwidth, m_clientheight,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		0));
	ID3D11Texture2D *pbackbuffer;
	HR(m_swapchain->GetBuffer(0,
		__uuidof(ID3D11Texture2D),
		reinterpret_cast<void**>(&pbackbuffer)));
	HR(m_d3ddevice->CreateRenderTargetView(pbackbuffer,
		0,
		&m_rendertargetview));
	ReleaseCOM(pbackbuffer);

	D3D11_TEXTURE2D_DESC depthstencil_desc;
	depthstencil_desc.Width = m_clientwidth;
	depthstencil_desc.Height = m_clientheight;
	depthstencil_desc.MipLevels = 1;
	depthstencil_desc.ArraySize = 1;
	depthstencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	if (m_enable4xMsaa)
	{
		depthstencil_desc.SampleDesc.Count = 4;
		depthstencil_desc.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		depthstencil_desc.SampleDesc.Count = 1;
		depthstencil_desc.SampleDesc.Quality = 0;
	}

	depthstencil_desc.Usage = D3D11_USAGE_DEFAULT;
	depthstencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthstencil_desc.CPUAccessFlags = 0;
	depthstencil_desc.MiscFlags = 0;

	HR(m_d3ddevice->CreateTexture2D(&depthstencil_desc, 0, &m_depthstencilbuffer));
	HR(m_d3ddevice->CreateDepthStencilView(m_depthstencilbuffer, 0, &m_depthstencilview));
	
	m_d3dimmediatecontext->OMSetRenderTargets(1,
		&m_rendertargetview,
		m_depthstencilview);

	m_screenviewport.TopLeftX = 0;
	m_screenviewport.TopLeftY = 0;
	m_screenviewport.Width = static_cast<float>(m_clientwidth);
	m_screenviewport.Height = static_cast<float>(m_clientheight);
	m_screenviewport.MinDepth = 0.0f;
	m_screenviewport.MaxDepth = 1.0f;

	m_d3dimmediatecontext->RSSetViewports(1, &m_screenviewport);
}



bool CGameStub::InitDirect3D()
{
	u32 createdevice_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	createdevice_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL feature_level;
	HRESULT hr = D3D11CreateDevice(0,
		m_d3ddrivertype,
		0,
		createdevice_flags,
		0,
		0,
		D3D11_SDK_VERSION,
		&m_d3ddevice,
		&feature_level,
		&m_d3dimmediatecontext);

	if (FAILED(hr))
	{
		MessageBox(0, L"D3D11CreateDevice Failed.", NULL, 0);
		return false;
	}

	if (feature_level != D3D_FEATURE_LEVEL_11_0)
	{
		MessageBox(0, L"Direct3D Feature Level 11 unsupported.", NULL, 0);
		return false;
	}

	HR(m_d3ddevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m_4xMsaaQuality));
	assert(m_4xMsaaQuality > 0);

	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = m_clientwidth;
	sd.BufferDesc.Height = m_clientheight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	if (m_enable4xMsaa)
	{
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m_4xMsaaQuality - 1;
	}
	else
	{
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
	}
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = m_mainhwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	IDXGIDevice *pdxgidevice = NULL;
	HR(m_d3ddevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pdxgidevice));
	
	IDXGIAdapter *pdxgiadapter = NULL;
	HR(pdxgidevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pdxgiadapter));
	
	IDXGIFactory *pdxgifactory = NULL;
	HR(pdxgiadapter->GetParent(__uuidof(IDXGIFactory),
		(void**)&pdxgifactory));

	HR(pdxgifactory->CreateSwapChain(m_d3ddevice, &sd, &m_swapchain));

	ReleaseCOM(pdxgifactory);
	ReleaseCOM(pdxgiadapter);
	ReleaseCOM(pdxgidevice);

	OnResize();

	return true;
}

void CGameStub::VCalculateFrameStats()
{
	static std::wstring init_capation = m_mainwndcaption;
	static u32 frame_cnt = 0;
	static float time_elapsed = 0.0f;
	++frame_cnt;
	float time_now = m_timer->VGetTime();
	if (time_now - time_elapsed >= 1000)
	{
		float fps = (float)frame_cnt;
		float msperframe = 1000.0f / fps;
		std::wostringstream outs;
		outs.precision(6);
		outs << L"FPS:  " << fps << L"   "
			<< L"Frame Time:   " << msperframe << L"  (ms)";
		m_mainwndcaption = init_capation + std::move(outs.str());
		frame_cnt = 0;
		time_elapsed += time_now - time_elapsed;
	}
}

_XE_END
