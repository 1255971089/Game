#include<Common/DDSTextureLoader.h>
#include<D3DX11.h>
#include<DirectXMath.h>
#include<Common/CGeometryGenerator.h>
#include<Common/global.h>
#include<Common/CD3DApp.h>
#include<Common/d3dUtil.h>
#include<Common/Camera.h>
#include<Common/LightHelper.h>


_XE_BEGIN


class CHillApp :public CD3DApp
{
public:
	struct Vertex
	{
		XMFLOAT3 pos;
		XMFLOAT4 color;
	};
public:
	CHillApp(HINSTANCE hinstance) :CD3DApp(hinstance),
		m_pVertexBuffer(NULL),
		m_pIndexBuffer(NULL),
		m_fx(NULL),
		m_tech(NULL),
		m_fxworldviewproj(NULL),
		m_inputlayout(NULL),
		m_gridVStart(0),
		m_gridIStart(0),
		m_boxVStart(0),
		m_boxIStart(0),
		m_WheelVStart(0),
		m_WheelIStart(0),
		m_AxleVStart(0),
		m_AxleIStart(0),
		m_gridindexcount(0),
		m_theta(1.5f*XM_PI),
		m_phi(0.1f*XM_PI),
		m_radius(200.0f)
	{
		m_mainwndcaption = L"CarGame";
		m_lastmousepos.x = 0;
		m_lastmousepos.y = 0;
		
		//我们把摄像机初始位置设在[0,f,2.f,-10.f]，默认朝向为z轴正方向
		m_camera.SetPosition(0.f, 2.f, -10.f);
		//设置好投影相关参数
		m_camera.SetLens(XM_PIDIV4, GetAspectRatio(), 1.f, 1000.f);

		//"三点式"照明
		//主光源
		m_dirLights[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		m_dirLights[0].diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_dirLights[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_dirLights[0].direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		//侧光源
		m_dirLights[1].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[1].diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
		m_dirLights[1].specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
		m_dirLights[1].direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		//背光源
		m_dirLights[2].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[2].diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_dirLights[2].specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[2].direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
		//材质
		m_materialBox.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_materialBox.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
		m_materialBox.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);

		m_materialGrid.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_materialGrid.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
		m_materialGrid.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
		//初始化各自的世界变换矩阵及其逆+转置矩阵、纹理坐标变换矩阵
		//这些矩阵是固定不变的，因此一次初始化
		//地面
		XMMATRIX gridWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&m_GridWorld, gridWorld);
		XMVECTOR det = XMMatrixDeterminant(gridWorld);
		XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, gridWorld));
		XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
		//车身
		XMMATRIX boxWorld = XMMatrixTranslation(0.f, 6.f, 0.f);
		XMStoreFloat4x4(&m_BoxWorld, boxWorld);
		det = XMMatrixDeterminant(boxWorld);
		worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, boxWorld));
		XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
		//车轮
		for (UINT i = 0; i < 2; ++i)
		{
			for (UINT j = 0; j < 2; ++j)
			{
				XMMATRIX wheelWorld = XMMatrixMultiply(XMMatrixRotationZ(XM_PI / 2), XMMatrixTranslation(-6.f + i * 12.f, 4.0f, -6.f + j * 12.f));
				XMStoreFloat4x4(&m_WheelWorld[i * 2 + j], wheelWorld);
				det = XMMatrixDeterminant(wheelWorld);
				worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, wheelWorld));
				XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
				XMStoreFloat4x4(&m_texTransWheel[i * 2 + j], XMMatrixIdentity());
			}
		}
		//车轴
		for (UINT i = 0; i < 2; ++i)
		{
			XMMATRIX axleWorld = XMMatrixMultiply(XMMatrixRotationZ(XM_PI / 2), XMMatrixTranslation(0.f, 4.f, -6.f + i * 12.f));
			XMStoreFloat4x4(&m_AxleWorld[i], axleWorld);
			det = XMMatrixDeterminant(axleWorld);
			worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, axleWorld));
			XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
			XMStoreFloat4x4(&m_texTransAxle[i], XMMatrixIdentity());
		}
		XMStoreFloat4x4(&m_texTransBox, XMMatrixIdentity());
		XMStoreFloat4x4(&m_texTransGrid, XMMatrixIdentity());
	}


	~CHillApp()
	{
		ReleaseCOM(m_pVertexBuffer);
		ReleaseCOM(m_pIndexBuffer);
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
	void BuildFX();
	void BuildVertexLayout();
	void BuildShape();
	f32 GetHeight(float x, float z)const;

private:
	//顶点和索引缓冲区
	//把所有图形的顶点、索引都放在一个缓冲区里面
	//绘制图形时指定其在缓冲区的起始位置及索引个数来找到对应的缓冲区位置
	ID3D11Buffer*							m_pVertexBuffer;
	ID3D11Buffer*							m_pIndexBuffer;
	ID3D11InputLayout*						m_inputlayout;
	
	ID3DX11Effect*							m_fx;
	ID3DX11EffectTechnique*					m_tech;
	ID3DX11EffectShaderResourceVariable*	m_pFxTex;
	ID3D11ShaderResourceView*				m_pSRVBox;		//箱子纹理
	ID3D11ShaderResourceView*				m_pSRVGrid;		//地板纹理
	//光源
	//3个平行光源
	Lights::DirectionalLight				m_dirLights[3];
	Lights::SpotLight						m_spotLight;
	//XMFLOAT3						m_eyePosW;
	//材质
	Lights::Material						m_materialBox;	//车身材质
	Lights::Material						m_materialGrid;//地面材质

	ID3DX11EffectVariable*			m_pFxSpotLight;
	ID3DX11EffectVectorVariable*	m_pFxEyePosW;


	//每个几何体在顶点索引缓冲区的起始位置
	UINT							m_gridVStart, m_gridIStart;
	UINT							m_boxVStart, m_boxIStart;
	UINT							m_WheelVStart, m_WheelIStart;
	UINT							m_AxleVStart, m_AxleIStart;

	//绘制的几种几何图形
	CGeometryGenerator::MeshData		m_grid;//底面
	CGeometryGenerator::MeshData		m_box;//车身
	CGeometryGenerator::MeshData		m_Wheel;//车轮
	CGeometryGenerator::MeshData		m_Axle;//车轴

	ID3D11ShaderResourceView*			m_pTexSRV;
	ID3DX11EffectMatrixVariable*		m_fxworldviewproj;

	
	//不同几何体的世界变换矩阵
	//4个车轮
	XMFLOAT4X4						m_WheelWorld[4];
	XMFLOAT4X4						m_worldViewProjWheel[4];
	XMFLOAT4X4						m_worldInvTransposeWheel[4];
	XMFLOAT4X4						m_texTransWheel[4];
	//2个轮轴
	XMFLOAT4X4						m_AxleWorld[2];
	XMFLOAT4X4						m_worldViewProjAxle[2];
	XMFLOAT4X4						m_worldInvTransposeAxle[2];
	XMFLOAT4X4						m_texTransAxle[2];
	//地形
	XMFLOAT4X4						m_GridWorld;
	XMFLOAT4X4						m_worldViewProjGrid;
	XMFLOAT4X4						m_worldInvTransposeGrid;
	XMFLOAT4X4						m_texTransGrid;
	//车身
	XMFLOAT4X4						m_BoxWorld;//世界变换矩阵
	XMFLOAT4X4						m_worldViewProjBox;			//世界视角投影矩阵
	XMFLOAT4X4						m_worldInvTransposeBox;		//世界逆矩阵的转置，用于法线变换
	XMFLOAT4X4						m_texTransBox;				//纹理坐标变换矩阵


	XMFLOAT4X4						m_view;
	XMFLOAT4X4						m_proj;

	u32 m_gridindexcount;
	//摄像机
	Camera		m_camera;
	float m_theta;
	float m_phi;
	float m_radius;

	POINT m_lastmousepos;
};

_XE_END