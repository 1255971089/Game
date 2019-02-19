#include "CHillApp.h"
#include<Common/MathHelper.h>
#include<Common/CGeometryGenerator.h>
#include<Common/d3dUtil.h>
#include<Common/LightHelper.h>
#include<Common/Effects.h>
_XE_BEGIN

bool CHillApp::VInit()
{
	if (!CD3DApp::VInit())
		return false;
	if (!Effects::InitAll(m_d3ddevice))
		return false;
	BuildShape();
	BuildFX();
	BuildVertexLayout();

	VOnResize();

	return true;
}

//转换到透视坐标
void CHillApp::VOnResize()
{
	CD3DApp::VOnResize();
	XMMATRIX proj = XMMatrixPerspectiveFovLH(0.25f*XM_PI,
		GetAspectRatio(),
		1.0f,
		1000.0f);
	XMStoreFloat4x4(&m_proj, proj);
}


//转换到相机坐标系
void CHillApp::VUpdateScene(float dt)
{
		XMMATRIX worldBox = XMLoadFloat4x4(&m_BoxWorld);
		XMMATRIX worldFloor = XMLoadFloat4x4(&m_GridWorld);

		float x = m_radius * sinf(m_phi)*cosf(m_theta);
		float z = m_radius * sinf(m_phi)*sinf(m_theta);
		float y = m_radius * cosf(m_phi);

		XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX V = XMMatrixLookAtLH(pos, target, up);
		XMStoreFloat4x4(&m_view, V);

		//计算投影矩阵
		XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, GetAspectRatio(), 1.0f, 1000.0f);
		XMStoreFloat4x4(&m_worldViewProjBox, worldBox*V*proj);

		XMStoreFloat4x4(&m_worldViewProjGrid, worldFloor*V*proj);
		//设置光源
		Effects::ms_pBasicEffect->m_pFxDirLights->SetRawValue(&m_dirLights, 0, 3 * sizeof(Lights::DirectionalLight));
		//设置观察点
		XMFLOAT3 eye;
		XMStoreFloat3(&eye, pos);
		Effects::ms_pBasicEffect->m_pFxEyePos->SetRawValue(&eye, 0, sizeof(eye));
}



void CHillApp::VDrawScene()
{
	m_d3dimmediatecontext->ClearRenderTargetView(m_rendertargetview, reinterpret_cast<const float*>(&Colors::LightSteelBlue));
	m_d3dimmediatecontext->ClearDepthStencilView(m_depthstencilview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_d3dimmediatecontext->IASetInputLayout(m_inputlayout);

	u32 stride = sizeof(Vertex);
	u32 offset = 0;

	m_d3dimmediatecontext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_d3dimmediatecontext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_d3dimmediatecontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//设置为线框绘制模式
	D3D11_RASTERIZER_DESC rsDesc;
	ZeroMemory(&rsDesc, sizeof(rsDesc));
	rsDesc.CullMode = D3D11_CULL_BACK;
	rsDesc.DepthClipEnable = true;
	//D3D11_FILL_WIREFRAME以线框模式绘制，D3D11_FILL_SOLID是以实体模式绘制
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	rsDesc.FrontCounterClockwise = false;
	ID3D11RasterizerState *rsState(nullptr);
	m_d3ddevice->CreateRasterizerState(&rsDesc, &rsState);
	m_d3dimmediatecontext->RSSetState(rsState);
	
	XMMATRIX view = XMLoadFloat4x4(&m_view);
	XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	D3DX11_TECHNIQUE_DESC tech_desc;
	m_tech->GetDesc(&tech_desc);


	//绘制网格
	XMMATRIX world = XMLoadFloat4x4(&m_GridWorld);
	XMMATRIX wordviewproj = world * view *proj;
	
	for (UINT i = 0; i < tech_desc.Passes; ++i)
	{
		/*
		Effects::ms_pBasicEffect->m_pFxWorld->SetMatrix(reinterpret_cast<const float*>(&m_GridWorld));
		Effects::ms_pBasicEffect->m_pFxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&m_worldViewProjGrid));
		Effects::ms_pBasicEffect->m_pFxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&m_worldInvTransposeGrid));
		Effects::ms_pBasicEffect->m_pFxMaterial->SetRawValue(&m_materialGrid, 0, sizeof(m_materialGrid));
		Effects::ms_pBasicEffect->m_pFxTexTrans->SetMatrix(reinterpret_cast<const float*>(&m_texTransGrid));
		Effects::ms_pBasicEffect->m_pFxSR->SetResource(m_pSRVGrid);
		*/
		m_fxworldviewproj->SetMatrix(reinterpret_cast<float*>(&wordviewproj));
		m_tech->GetPassByIndex(i)->Apply(0, m_d3dimmediatecontext);
		m_d3dimmediatecontext->DrawIndexed(m_grid.indices.size(), m_gridIStart, m_gridVStart);
		m_d3dimmediatecontext->RSSetState(0);
	}

	//绘制立方体(车身)
	world = XMLoadFloat4x4(&m_BoxWorld);
	wordviewproj = world * view *proj;
	for (UINT i = 0; i < tech_desc.Passes; ++i)
	{
		/*
		Effects::ms_pBasicEffect->m_pFxWorld->SetMatrix(reinterpret_cast<const float*>(&m_BoxWorld));
		Effects::ms_pBasicEffect->m_pFxWorldViewProj->SetMatrix(reinterpret_cast<const float*>(&m_worldViewProjBox));
		Effects::ms_pBasicEffect->m_pFxWorldInvTranspose->SetMatrix(reinterpret_cast<const float*>(&m_worldInvTransposeBox));
		Effects::ms_pBasicEffect->m_pFxMaterial->SetRawValue(&m_materialBox, 0, sizeof(m_materialBox));
		Effects::ms_pBasicEffect->m_pFxTexTrans->SetMatrix(reinterpret_cast<const float*>(&m_texTransBox));
		Effects::ms_pBasicEffect->m_pFxSR->SetResource(m_pSRVBox);
		*/
		m_fxworldviewproj->SetMatrix(reinterpret_cast<float*>(&wordviewproj));
		m_tech->GetPassByIndex(i)->Apply(0, m_d3dimmediatecontext);
		m_d3dimmediatecontext->DrawIndexed(m_box.indices.size(), m_boxIStart, m_boxVStart);
		m_d3dimmediatecontext->RSSetState(0);
	}
	
	//绘制四个车轮
	for (UINT i = 0; i < 4; ++i)
	{
		world = XMLoadFloat4x4(&m_WheelWorld[i]);
		wordviewproj = world * view *proj;
		for (UINT j = 0; j < tech_desc.Passes; ++j)
		{
			m_fxworldviewproj->SetMatrix(reinterpret_cast<float*>(&wordviewproj));
			m_tech->GetPassByIndex(j)->Apply(0, m_d3dimmediatecontext);
			m_d3dimmediatecontext->DrawIndexed(m_Wheel.indices.size(), m_WheelIStart, m_WheelVStart);
			m_d3dimmediatecontext->RSSetState(0);
		}
	}

	//绘制两个车轴
	for (UINT i = 0; i < 2; ++i)
	{
		world = XMLoadFloat4x4(&m_AxleWorld[i]);
		wordviewproj = world * view *proj;
		for (UINT j = 0; j < tech_desc.Passes; ++j)
		{
			m_fxworldviewproj->SetMatrix(reinterpret_cast<float*>(&wordviewproj));
			m_tech->GetPassByIndex(j)->Apply(0, m_d3dimmediatecontext);
			m_d3dimmediatecontext->DrawIndexed(m_Axle.indices.size(), m_AxleIStart, m_AxleVStart);
			m_d3dimmediatecontext->RSSetState(0);
		}
	}
	




	HR(m_swapchain->Present(0, 0));

}

void CHillApp::VOnMouseDown(WPARAM btn_state, s32 x, s32 y)
{
	m_lastmousepos.x = x;
	m_lastmousepos.y = y;

	SetCapture(m_mainhwnd);
}

void CHillApp::VOnMouseUp(WPARAM btn_state, s32 x, s32 y)
{
	ReleaseCapture();
}

void CHillApp::VOnMouseMove(WPARAM btn_state, s32 x, s32 y)
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
		float dx = 0.2f*(x - m_lastmousepos.x);
		float dy = 0.2f *(y - m_lastmousepos.y);
		m_radius += dx - dy;
		m_radius = math::Clamp(m_radius, 50.0f, 500.0f);
	}
	m_lastmousepos.x = x;
	m_lastmousepos.y = y;
}

void CHillApp::BuildShape()
{

	CGeometryGenerator::GetInstance()->CreateGrid(160.f, 160.f, 50, 50, m_grid);
	CGeometryGenerator::GetInstance()->CreateBox(10, 10.f, 20, m_box);
	CGeometryGenerator::GetInstance()->CreateCylinder(4.f, 4.f, 1, 40, 40, m_Wheel);
	CGeometryGenerator::GetInstance()->AddCylinderTopCap(4.f, 4.f, 1, 40, 40, m_Wheel);
	CGeometryGenerator::GetInstance()->AddCylinderBottomCap(4.f, 4.f, 1, 40, 40, m_Wheel);
	CGeometryGenerator::GetInstance()->CreateCylinder(1.f, 1.f, 14, 40, 40, m_Axle);
	CGeometryGenerator::GetInstance()->AddCylinderTopCap(1.f, 1.f, 14, 40, 40, m_Axle);
	CGeometryGenerator::GetInstance()->AddCylinderBottomCap(1.f, 1.f, 14, 40, 40, m_Axle);

	m_gridVStart = 0;
	m_gridIStart = 0;
	m_boxVStart = m_grid.vertices.size();
	m_boxIStart = m_grid.indices.size();
	m_WheelVStart = m_boxVStart + m_box.vertices.size();
	m_WheelIStart = m_boxIStart + m_box.indices.size();
	m_AxleVStart = m_WheelVStart + m_Wheel.vertices.size();
	m_AxleIStart = m_WheelIStart + m_Wheel.indices.size();
	UINT totalVertices = m_AxleVStart + m_Axle.vertices.size();
	UINT totalIndices = m_AxleIStart + m_Axle.indices.size();
	std::vector<Vertex> vertices(totalVertices);

	for (UINT i = 0; i < m_grid.vertices.size(); ++i)
	{
		vertices[m_gridVStart + i].pos = m_grid.vertices[i].position;
		vertices[m_gridVStart + i].color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}

	for (UINT i = 0; i < m_box.vertices.size(); ++i)
	{
		vertices[m_boxVStart + i].pos = m_box.vertices[i].position;
		vertices[m_boxVStart + i].color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}

	for (UINT i = 0; i < m_Wheel.vertices.size(); ++i)
	{
		vertices[m_WheelVStart + i].pos = m_Wheel.vertices[i].position;
		vertices[m_WheelVStart + i].color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}

	for (UINT i = 0; i < m_Axle.vertices.size(); ++i)
	{
		vertices[m_AxleVStart + i].pos = m_Axle.vertices[i].position;
		vertices[m_AxleVStart + i].color = XMFLOAT4(0.f, 0.f, 0.f, 1.f);
	}

	D3D11_BUFFER_DESC vertexDesc;
	vertexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexDesc.ByteWidth = sizeof(Vertex) * totalVertices;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = 0;
	vertexDesc.MiscFlags = 0;
	vertexDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA resourceData;
	resourceData.pSysMem = &vertices[0];
	HR(m_d3ddevice->CreateBuffer(&vertexDesc, &resourceData, &m_pVertexBuffer));


	//create index buffer
	std::vector<UINT> indices(totalIndices);
	for (UINT i = 0; i < m_grid.indices.size(); ++i)
	{
		indices[m_gridIStart + i] = m_grid.indices[i];
	}
	for (UINT i = 0; i < m_box.indices.size(); ++i)
	{
		indices[m_boxIStart + i] = m_box.indices[i];
	}
	for (UINT i = 0; i < m_Wheel.indices.size(); ++i)
	{
		indices[m_WheelIStart + i] = m_Wheel.indices[i];
	}
	for (UINT i = 0; i < m_Axle.indices.size(); ++i)
	{
		indices[m_AxleIStart + i] = m_Axle.indices[i];
	}

	D3D11_BUFFER_DESC indexDesc;
	indexDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexDesc.ByteWidth = sizeof(u32) *totalIndices;
	indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexDesc.CPUAccessFlags = 0;
	indexDesc.MiscFlags = 0;
	indexDesc.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = &indices[0];
	HR(m_d3ddevice->CreateBuffer(&indexDesc, &indexData, &m_pIndexBuffer));


}

void CHillApp::BuildFX()
{
	std::ifstream fin("fx/color.fxo", std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	u32 size = (u32)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	std::vector<char> compiled_shader(size);
	fin.read(&compiled_shader[0], size);
	HR(D3DX11CreateEffectFromMemory(&compiled_shader[0], size,
		0, m_d3ddevice, &m_fx));


	DirectX::CreateDDSTextureFromFile(m_d3ddevice, L"Textures/Wood.dds", nullptr, &m_pSRVBox);
	DirectX::CreateDDSTextureFromFile(m_d3ddevice, L"Textures/checkboard.dds", nullptr, &m_pSRVGrid);
	m_tech = m_fx->GetTechniqueByName("ColorTech");
	//m_pFxSpotLight = m_fx->GetVariableByName("gSpotLight");
	//m_pFxEyePosW = m_fx->GetVariableByName("gEyePosW")->AsVector();
	m_fxworldviewproj = m_fx->GetVariableByName("g_worldviewproj")->AsMatrix();
}

void CHillApp::BuildVertexLayout()
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

f32 CHillApp::GetHeight(float x, float z) const
{
	return 0.3f*(z*sinf(0.1f*x) + x * cosf(0.1f*z));
}

_XE_END