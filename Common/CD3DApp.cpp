#include "CD3DApp.h"

#include<windowsx.h>
_XE_BEGIN

namespace
{
	CD3DApp *g_d3dapp = nullptr;
	LRESULT CALLBACK MainWndProc(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam)
	{
		return g_d3dapp->VMsgProc(hwnd, msg, wparam, lparam);
	}
}


CD3DApp::CD3DApp(HINSTANCE hinstance):CGameStub(hinstance)
{
	g_d3dapp = this;
}

bool CD3DApp::VInit()
{
	if (!InitMainWindow())
		return false;
	if (!InitDirect3D())
		return false;
	return true;
}

s32 CD3DApp::VRun()
{
	MSG msg = { 0 };
	m_timer->VSetTime(0);
	m_timer->VStart();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			m_timer->VTick();
			if (!m_paused)
			{
				VCalculateFrameStats();
				VUpdateScene(m_timer->VGetDeltaTime());
				VDrawScene();
			}
			else
			{
				Sleep(100);
			}
		}
	}

	return s32(msg.wParam);
}

void CD3DApp::VOnResize()
{
	OnResize();
}

LRESULT CD3DApp::VMsgProc(HWND hwnd, u32 msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_ACTIVATE:
		if (LOWORD(wparam) == WA_INACTIVE)
		{
			m_paused = true;
			m_timer->VStop();
		}
		else
		{
			m_paused = false;
			m_timer->VStart();
		}
		return 0;
	case WM_SIZE:
		m_clientwidth = LOWORD(lparam);
		m_clientheight = HIWORD(lparam);
		if (m_d3ddevice)
		{
			if (wparam == SIZE_MINIMIZED)
			{
				m_paused = true;
				m_minimized = true;
				m_maximized = false;
			}
			else if (wparam == SIZE_MAXIMIZED)
			{
				m_paused = false;
				m_minimized = false;
				m_maximized = true;
				VOnResize();
			}
			else if (wparam == SIZE_RESTORED)
			{
				if (m_minimized)
				{
					m_paused = false;
					m_minimized = false;
					VOnResize();
				}
				else if(m_maximized)
				{
					m_paused = false;
					m_maximized = false;
					VOnResize();
				}
				else if (m_resizing)
				{
					//reserved
				}
				else
				{
					VOnResize();
				}
			}
		}
		return 0;
	case WM_ENTERSIZEMOVE:
		m_paused = true;
		m_resizing = true;
		m_timer->VStop();
		return 0;
	case WM_EXITSIZEMOVE:
		m_paused = false;
		m_resizing = false;
		m_timer->VStart();
		VOnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_MENUCHAR:
		return MAKELRESULT(0, MNC_CLOSE);
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lparam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lparam)->ptMinTrackSize.y = 200;
		return 0;
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		VOnMouseDown(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		VOnMouseUp(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	case WM_MOUSEMOVE:
		VOnMouseMove(wparam, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
		return 0;
	default:
		break;
	}
	return DefWindowProc(hwnd,msg,wparam,lparam);
}


bool CD3DApp::InitMainWindow()
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"D3DWndClassName";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"RegisterClass Failed.", 0, 0);
		return false;
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, m_clientwidth, m_clientheight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);

	int width = R.right - R.left;
	int height = R.bottom - R.top;

	m_mainhwnd = CreateWindow(L"D3DWndClassName", 
		m_mainwndcaption.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT, 
		width, height,
		0, 
		0, 
		m_hinstance, 
		0);
	if (!m_mainhwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	ShowWindow(m_mainhwnd, SW_SHOW);
	UpdateWindow(m_mainhwnd);
	return true;
}

_XE_END
