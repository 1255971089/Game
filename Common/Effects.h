#pragma once

#include <windows.h>
#include <string>
#include <d3dcompiler.h>
#include "d3dx11effect.h"
//基类
class Effect
{
public:
	Effect() : m_pFx(nullptr){}
	virtual ~Effect()
	{
		m_pFx->Release();
		m_pFx = nullptr;
	}

	//禁止复制
	Effect(const Effect&) = delete;
	Effect& operator=(const Effect&) = delete;

	//通过device和fx文件初始化
	virtual bool Init(ID3D11Device* device, std::wstring fileName);

	ID3DX11Effect* m_pFx;
};

class BasicEffect : public Effect
{
public:
	BasicEffect():m_pFxWorldViewProj(nullptr),
		m_pFxWorld(nullptr),
		m_pFxWorldInvTranspose(nullptr),
		m_pFxTexTrans(nullptr),
		m_pFxMaterial(nullptr),
		m_pFxDirLights(nullptr),
		m_pFxEyePos(nullptr),
		m_pFxSR(nullptr),
		m_pFxLight1Tech(nullptr),
		m_pFxLight2Tech(nullptr),
		m_pFxLight3Tech(nullptr),
		m_pFxLight1TexTech(nullptr),
		m_pFxLight2TexTech(nullptr),
		m_pFxLight3TexTech(nullptr)
	{}

	bool Init(ID3D11Device* device, std::wstring fileName) override;

	ID3DX11EffectMatrixVariable* m_pFxWorldViewProj;
	ID3DX11EffectMatrixVariable* m_pFxWorld;
	ID3DX11EffectMatrixVariable* m_pFxWorldInvTranspose;
	ID3DX11EffectMatrixVariable* m_pFxTexTrans;
	ID3DX11EffectVariable*	     m_pFxMaterial;

	ID3DX11EffectVariable*		 m_pFxDirLights;
	ID3DX11EffectVariable*		 m_pFxEyePos;

	ID3DX11EffectShaderResourceVariable*	m_pFxSR;

	ID3DX11EffectTechnique		*m_pFxLight1Tech;
	ID3DX11EffectTechnique		*m_pFxLight2Tech;
	ID3DX11EffectTechnique		*m_pFxLight3Tech;
	ID3DX11EffectTechnique		*m_pFxLight1TexTech;
	ID3DX11EffectTechnique		*m_pFxLight2TexTech;
	ID3DX11EffectTechnique		*m_pFxLight3TexTech;
};


class Effects
{
public:
	static bool InitAll(ID3D11Device* device);
	static void ReleaseAll();

	static BasicEffect* ms_pBasicEffect;
};

