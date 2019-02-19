#pragma once

#include"global.h"
#include"interface.h"
#include<string>
#include<d3d11.h>

_XE_BEGIN

//the common parts for game instance
class CGameStub
{
public:
	CGameStub(HINSTANCE hinstance);
	~CGameStub();

	HINSTANCE GetAppHinstance() const
	{
		return m_hinstance;
	}

	HWND GetMainHwnd() const
	{
		return m_mainhwnd;
	}

	float GetAspectRatio() const
	{
		return static_cast<float>(m_clientwidth) / m_clientheight;
	}
	
protected:
	bool InitDirect3D();

	virtual void VCalculateFrameStats();

	void OnResize();
protected:
	HINSTANCE m_hinstance;
	HWND m_mainhwnd;
	bool m_paused;
	bool m_minimized;
	bool m_maximized;
	bool m_resizing;
	u32 m_4xMsaaQuality;

	ITimer *m_timer;
	ID3D11Device *m_d3ddevice;
	ID3D11DeviceContext *m_d3dimmediatecontext;
	IDXGISwapChain *m_swapchain;
	ID3D11Texture2D *m_depthstencilbuffer;
	ID3D11RenderTargetView *m_rendertargetview;
	ID3D11DepthStencilView *m_depthstencilview;
	D3D11_VIEWPORT m_screenviewport;

	std::wstring m_mainwndcaption;
	D3D_DRIVER_TYPE m_d3ddrivertype;
	s32 m_clientwidth;
	s32 m_clientheight;
	bool m_enable4xMsaa;
};

_XE_END

