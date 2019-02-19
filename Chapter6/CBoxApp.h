#pragma once


#include<Common/global.h>
#include<Common/CD3DApp.h>
#include<Common/d3dUtil.h>
#include<D3DX11.h>
#include<d3dx11effect.h>
#include<xnamath.h>

_XE_BEGIN

class CBoxApp :public CD3DApp
{
public:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};
public:
	CBoxApp(HINSTANCE hinstance) :CD3DApp(hinstance),
		m_boxvb(NULL),
		m_boxib(NULL),
		m_fx(NULL),
		m_tech(NULL),
		m_fxworldviewproj(NULL),
		m_inputlayout(NULL),
		m_theta(1.5f*XM_PI),
		m_phi(0.25f*XM_PI),
		m_radius(5.0f)
	{
		m_mainwndcaption = L"Box Demo";
		m_lastmousepos.x = 0;
		m_lastmousepos.y = 0;

		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_world, I);
		XMStoreFloat4x4(&m_view, I);
		XMStoreFloat4x4(&m_proj, I);
	}

	~CBoxApp()
	{
		ReleaseCOM(m_boxvb);
		ReleaseCOM(m_boxib);
		ReleaseCOM(m_fx);
		ReleaseCOM(m_inputlayout);
	}

	virtual bool VInit() override;

	//virtual s32 VRun() override;

	virtual void VOnResize() override;

	virtual void VUpdateScene(float dt) override;

	virtual void VDrawScene() override;

	virtual void VOnMouseDown(WPARAM btn_state, s32 x, s32 y);
	virtual void VOnMouseUp(WPARAM btn_state, s32 x, s32 y);
	virtual void VOnMouseMove(WPARAM btn_state, s32 x, s32 y);
private:
	void BuildGeometryBuffers();
	void BuildFX();
	void BuildVertexLayout();
private:
	ID3D11Buffer *m_boxvb;
	ID3D11Buffer *m_boxib;

	ID3DX11Effect *m_fx;
	ID3DX11EffectTechnique *m_tech;
	ID3DX11EffectMatrixVariable *m_fxworldviewproj;

	ID3D11InputLayout *m_inputlayout;

	XMFLOAT4X4 m_world;
	XMFLOAT4X4 m_view;
	XMFLOAT4X4 m_proj;

	float m_theta;
	float m_phi;
	float m_radius;

	POINT m_lastmousepos;
};

_XE_END

