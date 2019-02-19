#include "CBoxApp.h"

#include"Common/MathHelper.h"

_XE_BEGIN

bool CBoxApp::VInit()
{
	if (!CD3DApp::VInit())
		return false;

	BuildGeometryBuffers();
	BuildFX();
	BuildVertexLayout();
	
	VOnResize();

	return true;
}

//s32 CBoxApp::VRun()
//{
//	return CD3DApp::VRun();
//}

void CBoxApp::VOnResize()
{
	CD3DApp::VOnResize();
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f*XM_PI,
		GetAspectRatio(),
		1.0f,
		1000.0f);
	XMStoreFloat4x4(&m_proj, proj);
}

void CBoxApp::VUpdateScene(float dt)
{
	float x = m_radius * sinf(m_phi)*cosf(m_theta);
	float z = m_radius * sinf(m_phi)*sinf(m_theta);
	float y = m_radius * cosf(m_phi);

	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&m_view, V);
}

void CBoxApp::VDrawScene()
{
	m_d3dimmediatecontext->ClearRenderTargetView(m_rendertargetview,
		reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	m_d3dimmediatecontext->ClearDepthStencilView(m_depthstencilview,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_d3dimmediatecontext->IASetInputLayout(m_inputlayout);
	m_d3dimmediatecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	u32 stride = sizeof(Vertex);
	u32 offset = 0;
	m_d3dimmediatecontext->IASetVertexBuffers(0, 1, &m_boxvb, &stride, &offset);
	m_d3dimmediatecontext->IASetIndexBuffer(m_boxib, DXGI_FORMAT_R32_UINT, 0);

	XMMATRIX world = XMLoadFloat4x4(&m_world);
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	XMMATRIX wordviewproj = world * view *proj;

	m_fxworldviewproj->SetMatrix(reinterpret_cast<float*>(&wordviewproj));

	D3DX11_TECHNIQUE_DESC tech_desc;
	m_tech->GetDesc(&tech_desc);
	for (u32 p = 0; p < tech_desc.Passes; ++p)
	{
		m_tech->GetPassByIndex(p)->Apply(0, m_d3dimmediatecontext);
		m_d3dimmediatecontext->DrawIndexed(36, 0, 0);
	}
	HR(m_swapchain->Present(0, 0));
}

void CBoxApp::VOnMouseDown(WPARAM btn_state, s32 x, s32 y)
{
	m_lastmousepos.x = x;
	m_lastmousepos.y = y;

	SetCapture(m_mainhwnd);
}

void CBoxApp::VOnMouseUp(WPARAM btn_state, s32 x, s32 y)
{
	ReleaseCapture();
}

void CBoxApp::VOnMouseMove(WPARAM btn_state, s32 x, s32 y)
{
	if (btn_state & MK_LBUTTON) 
	{
		float dx = XMConvertToRadians(0.25f*(x - m_lastmousepos.x));
		float dy = XMConvertToRadians(0.25f*(y - m_lastmousepos.y));

		m_theta -= dx;
		m_phi -= dy;
		m_phi = math::Clamp(m_phi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btn_state & MK_RBUTTON) != 0)
	{
		float dx = 0.005f*(x - m_lastmousepos.x);
		float dy = 0.005f *(y - m_lastmousepos.y);
		m_radius += dx - dy;
		m_radius = math::Clamp(m_radius, 3.0f, 15.0f);
	}
	m_lastmousepos.x = x;
	m_lastmousepos.y = y;
}

void CBoxApp::BuildGeometryBuffers()
{
	Vertex vertices[] =
	{
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f), (const float*)&Colors::White   },
		{ XMFLOAT3(-1.0f, +1.0f, -1.0f), (const float*)&Colors::Black   },
		{ XMFLOAT3(+1.0f, +1.0f, -1.0f), (const float*)&Colors::Red     },
		{ XMFLOAT3(+1.0f, -1.0f, -1.0f), (const float*)&Colors::Green   },
		{ XMFLOAT3(-1.0f, -1.0f, +1.0f), (const float*)&Colors::Blue    },
		{ XMFLOAT3(-1.0f, +1.0f, +1.0f), (const float*)&Colors::Yellow  },
		{ XMFLOAT3(+1.0f, +1.0f, +1.0f), (const float*)&Colors::Cyan    },
		{ XMFLOAT3(+1.0f, -1.0f, +1.0f), (const float*)&Colors::Magenta }
	};

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Vertex) * 8;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA vinit_data;
	vinit_data.pSysMem = vertices;
	HR(m_d3ddevice->CreateBuffer(&vbd, &vinit_data, &m_boxvb));

	u32 indices[] = {
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(u32) * 36;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA iinit_data;
	iinit_data.pSysMem = indices;
	HR(m_d3ddevice->CreateBuffer(&ibd, &iinit_data, &m_boxib));
}

void CBoxApp::BuildFX()
{
	std::ifstream fin("fx/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	u32 size = (u32)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiled_shader(size);
	fin.read(&compiled_shader[0], size);
	HR(D3DX11CreateEffectFromMemory(&compiled_shader[0], size,
		0, m_d3ddevice, &m_fx));
	/*u32 shader_flags = 0;
#if defined(DEBUG) || defined(_DEBUG)
	shader_flags |= D3D10_SHADER_DEBUG;
	shader_flags |= D3D10_SHADER_SKIP_OPTIMIZATION;
#endif
	ID3D10Blob *pcompiled_shader = NULL;
	ID3D10Blob *pcompilation_msgs = NULL;
	HRESULT hr = D3DX11CompileFromFile(L"fx/color.fx",
		NULL, 0, 0,
		"fx_5_0",
		shader_flags,
		0, NULL,
		&pcompiled_shader,
		&pcompilation_msgs,
		NULL);
	if (pcompilation_msgs != 0)
	{
		MessageBoxA(0, (c8*)pcompilation_msgs->GetBufferPointer(), NULL, 0);
		ReleaseCOM(pcompilation_msgs);
	}
	if (FAILED(hr))
	{
		DXTrace(__FILE__, (u32)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}
	HR(D3DX11CreateEffectFromMemory(pcompiled_shader->GetBufferPointer(),
		pcompiled_shader->GetBufferSize(),
		0,
		m_d3ddevice,
		&m_fx));
	ReleaseCOM(pcompiled_shader);*/
	m_tech = m_fx->GetTechniqueByName("ColorTech");
	m_fxworldviewproj = m_fx->GetVariableByName("g_worldviewproj")->AsMatrix();
}

void CBoxApp::BuildVertexLayout()
{
	D3D11_INPUT_ELEMENT_DESC vertex_desc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC pass_desc;
	m_tech->GetPassByIndex(0)->GetDesc(&pass_desc);
	HR(m_d3ddevice->CreateInputLayout(vertex_desc,
		2,
		pass_desc.pIAInputSignature,
		pass_desc.IAInputSignatureSize,
		&m_inputlayout));
}

_XE_END
