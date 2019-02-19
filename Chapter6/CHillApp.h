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
		
		//���ǰ��������ʼλ������[0,f,2.f,-10.f]��Ĭ�ϳ���Ϊz��������
		m_camera.SetPosition(0.f, 2.f, -10.f);
		//���ú�ͶӰ��ز���
		m_camera.SetLens(XM_PIDIV4, GetAspectRatio(), 1.f, 1000.f);

		//"����ʽ"����
		//����Դ
		m_dirLights[0].ambient = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		m_dirLights[0].diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_dirLights[0].specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_dirLights[0].direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);
		//���Դ
		m_dirLights[1].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[1].diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
		m_dirLights[1].specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
		m_dirLights[1].direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);
		//����Դ
		m_dirLights[2].ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[2].diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		m_dirLights[2].specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		m_dirLights[2].direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
		//����
		m_materialBox.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_materialBox.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
		m_materialBox.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);

		m_materialGrid.ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		m_materialGrid.diffuse = XMFLOAT4(1.f, 1.f, 1.f, 1.0f);
		m_materialGrid.specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 16.0f);
		//��ʼ�����Ե�����任��������+ת�þ�����������任����
		//��Щ�����ǹ̶�����ģ����һ�γ�ʼ��
		//����
		XMMATRIX gridWorld = XMMatrixIdentity();
		XMStoreFloat4x4(&m_GridWorld, gridWorld);
		XMVECTOR det = XMMatrixDeterminant(gridWorld);
		XMMATRIX worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, gridWorld));
		XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
		//����
		XMMATRIX boxWorld = XMMatrixTranslation(0.f, 6.f, 0.f);
		XMStoreFloat4x4(&m_BoxWorld, boxWorld);
		det = XMMatrixDeterminant(boxWorld);
		worldInvTranspose = XMMatrixTranspose(XMMatrixInverse(&det, boxWorld));
		XMStoreFloat4x4(&m_worldInvTransposeBox, worldInvTranspose);
		//����
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
		//����
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
	//���������������
	//������ͼ�εĶ��㡢����������һ������������
	//����ͼ��ʱָ�����ڻ���������ʼλ�ü������������ҵ���Ӧ�Ļ�����λ��
	ID3D11Buffer*							m_pVertexBuffer;
	ID3D11Buffer*							m_pIndexBuffer;
	ID3D11InputLayout*						m_inputlayout;
	
	ID3DX11Effect*							m_fx;
	ID3DX11EffectTechnique*					m_tech;
	ID3DX11EffectShaderResourceVariable*	m_pFxTex;
	ID3D11ShaderResourceView*				m_pSRVBox;		//��������
	ID3D11ShaderResourceView*				m_pSRVGrid;		//�ذ�����
	//��Դ
	//3��ƽ�й�Դ
	Lights::DirectionalLight				m_dirLights[3];
	Lights::SpotLight						m_spotLight;
	//XMFLOAT3						m_eyePosW;
	//����
	Lights::Material						m_materialBox;	//�������
	Lights::Material						m_materialGrid;//�������

	ID3DX11EffectVariable*			m_pFxSpotLight;
	ID3DX11EffectVectorVariable*	m_pFxEyePosW;


	//ÿ���������ڶ�����������������ʼλ��
	UINT							m_gridVStart, m_gridIStart;
	UINT							m_boxVStart, m_boxIStart;
	UINT							m_WheelVStart, m_WheelIStart;
	UINT							m_AxleVStart, m_AxleIStart;

	//���Ƶļ��ּ���ͼ��
	CGeometryGenerator::MeshData		m_grid;//����
	CGeometryGenerator::MeshData		m_box;//����
	CGeometryGenerator::MeshData		m_Wheel;//����
	CGeometryGenerator::MeshData		m_Axle;//����

	ID3D11ShaderResourceView*			m_pTexSRV;
	ID3DX11EffectMatrixVariable*		m_fxworldviewproj;

	
	//��ͬ�����������任����
	//4������
	XMFLOAT4X4						m_WheelWorld[4];
	XMFLOAT4X4						m_worldViewProjWheel[4];
	XMFLOAT4X4						m_worldInvTransposeWheel[4];
	XMFLOAT4X4						m_texTransWheel[4];
	//2������
	XMFLOAT4X4						m_AxleWorld[2];
	XMFLOAT4X4						m_worldViewProjAxle[2];
	XMFLOAT4X4						m_worldInvTransposeAxle[2];
	XMFLOAT4X4						m_texTransAxle[2];
	//����
	XMFLOAT4X4						m_GridWorld;
	XMFLOAT4X4						m_worldViewProjGrid;
	XMFLOAT4X4						m_worldInvTransposeGrid;
	XMFLOAT4X4						m_texTransGrid;
	//����
	XMFLOAT4X4						m_BoxWorld;//����任����
	XMFLOAT4X4						m_worldViewProjBox;			//�����ӽ�ͶӰ����
	XMFLOAT4X4						m_worldInvTransposeBox;		//����������ת�ã����ڷ��߱任
	XMFLOAT4X4						m_texTransBox;				//��������任����


	XMFLOAT4X4						m_view;
	XMFLOAT4X4						m_proj;

	u32 m_gridindexcount;
	//�����
	Camera		m_camera;
	float m_theta;
	float m_phi;
	float m_radius;

	POINT m_lastmousepos;
};

_XE_END