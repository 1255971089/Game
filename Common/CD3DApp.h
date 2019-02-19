#pragma once

#include"global.h"
#include"interface.h"
#include"CGameStub.h"
_XE_BEGIN

class CD3DApp :public IGame, public CGameStub
{
public:
	CD3DApp(HINSTANCE hinstance);

	virtual ~CD3DApp()
	{}

	virtual bool VInit() override;

	virtual s32 VRun() override;

	virtual void VOnResize() override;
	
	virtual LRESULT VMsgProc(HWND hwnd,
		u32 msg,
		WPARAM wparam,
		LPARAM lparam);

	virtual void VOnMouseDown(WPARAM btn_state, s32 x, s32 y) {};
	virtual void VOnMouseUp(WPARAM btn_state, s32 x, s32 y) {};
	virtual void VOnMouseMove(WPARAM btn_state, s32 x, s32 y) {};
protected:
	bool InitMainWindow();

	virtual void VCalculateFrameStats() override
	{
		CGameStub::VCalculateFrameStats();
		SetWindowText(m_mainhwnd,m_mainwndcaption.c_str());
	}
};

_XE_END