#include "Effects.h"
#include<DxErr.h>
#include<effects.h>
#include<Common/d3dUtil.h>
#include<Common/CGameStub.h>

bool Effect::Init(ID3D11Device* device, std::wstring fileName)
{
	/*
	HRESULT hr;
	ID3DBlob* errorBlob;
	DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined _DEBUG || defined DEBUG
	shaderFlags = D3DCOMPILE_DEBUG;
#endif
	hr = D3DX11CreateEffectFromMemory(fileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, shaderFlags,
		0, device, &m_pFx, &errorBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"create shader failed!", L"error", MB_OK);
		return hr;
	}
	*/
	DWORD shaderFlags = 0; 
#if defined (DEBUG) || defined(_DEBUG)
		shaderFlags |= D3D10_SHADER_DEBUG; 
		shaderFlags |= D3D10_SHADER_SKIP_OPTIMIZATION; 
#endif 
	   ID3D10Blob * compiledShader = 0; 
	   ID3D10Blob * compilationMsgs = 0; 
	   HRESULT hr = D3DX11CompileFromFile(L"FX/Basic.fx", 0, 0, 0, "fx_5_0", shaderFlags, 0, 0, &compiledShader, &compilationMsgs, 0);
	// compilationMsgs包含错误或警告的信息 
	   if(compilationMsgs != 0) 
	   {
		   MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0, 0);
		   ReleaseCOM(compilationMsgs); 
	   }
	// 就算没有 compilationMsgs，也需要确保没有其他错误 
	   if(FAILED(hr)) 
	   { 
		   DXTrace(__FILE__,(DWORD)__LINE__,hr,L"D3DX11Compile FromFile",true); 
		   return hr;
	   }
	hr=D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(),
		compiledShader->GetBufferSize(),
		0, device, &m_pFx);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"create shader failed!", L"error", MB_OK);
		return hr;
	}
	// 编译完成释放资源 
	ReleaseCOM(compiledShader);
	return true;
}

bool BasicEffect::Init(ID3D11Device* device, std::wstring fileName)
{
	if (!Effect::Init(device, fileName))
	{
		MessageBox(nullptr, L"create basic shader failed!", L"error", MB_OK);
	}
	m_pFxWorldViewProj = m_pFx->GetVariableByName("g_worldViewProj")->AsMatrix();
	m_pFxWorld = m_pFx->GetVariableByName("g_world")->AsMatrix();
	m_pFxWorldInvTranspose = m_pFx->GetVariableByName("g_worldInvTranspose")->AsMatrix();
	m_pFxTexTrans = m_pFx->GetVariableByName("g_texTrans")->AsMatrix();
	m_pFxMaterial = m_pFx->GetVariableByName("g_material");

	m_pFxDirLights = m_pFx->GetVariableByName("g_lights");
	m_pFxEyePos = m_pFx->GetVariableByName("g_eyePos");

	m_pFxSR = m_pFx->GetVariableByName("g_tex")->AsShaderResource();

	m_pFxLight1Tech = m_pFx->GetTechniqueByName("Light1");
 	m_pFxLight2Tech = m_pFx->GetTechniqueByName("Light2");
	m_pFxLight3Tech = m_pFx->GetTechniqueByName("Light3");
	m_pFxLight1TexTech = m_pFx->GetTechniqueByName("Light1Tex");
	m_pFxLight2TexTech = m_pFx->GetTechniqueByName("Light2Tex");
	m_pFxLight3TexTech = m_pFx->GetTechniqueByName("Light3Tex");

	return true;
}

BasicEffect* Effects::ms_pBasicEffect{nullptr};
bool Effects::InitAll(ID3D11Device* device)
{
	if (!ms_pBasicEffect)
	{
		ms_pBasicEffect = new BasicEffect;
		return ms_pBasicEffect->Init(device, L"fx/basic.fx");
	}
	return true;
}

void Effects::ReleaseAll()
{
	if (ms_pBasicEffect)
	{
		delete ms_pBasicEffect;
		ms_pBasicEffect = nullptr;
	}
}
