#include "LightHelper.fx"

//���ÿ������
cbuffer PerObject
{
	float4x4 g_worldViewProj;       //���� �ӽ� ͶӰ����
	float4x4 g_world;               //�������
	float4x4 g_worldInvTranspose;   //�������������ת�ã����ڵ�������
	Material g_material;            //����
	float4x4 g_texTrans;            //����任
};

//���ÿһ֡
cbuffer PerFrame
{
	DirectionalLight g_lights[3]; //��Դ����
	float3 g_eyePos; //�۲��
};

//����
Texture2D g_tex;

SamplerState samplerTex
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 pos      : POSITION;     //��������
	float3 normal   : NORMAL;        //���㷨��
	float2 tex      : TEXCOORD;      //��������
};

struct VertexOut
{
	float3 posTrans : POSITION; //����任������꣬����world����
	float4 posH : SV_Position;   //ͶӰ�������
	float3 normal : NORMAL;      //����任��Ķ��㷨��
	float2 tex : TEXCOORD;       //ת�������������
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	//����任��Ķ�������
	vout.posTrans = mul(float4(vin.pos, 1.0f), g_world).xyz;
	//ͶӰ�任��Ķ�������
	vout.posH = mul(float4(vin.pos, 1.0f), g_worldViewProj);
	//�任��ķ�������
	vout.normal = mul(float4(vin.normal, 1.0f), g_worldInvTranspose).xyz;
	//�任�����������
	vout.tex = mul(float4(vin.tex, 0.0f, 1.0f), g_texTrans).xy;

	return vout;
}

float4 PS(VertexOut pin, uniform int numLights, uniform bool useTexture) :SV_Target
{
	//��ʼ��ɫΪ��ɫ
	float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
	//ʹ������������Ӧ��ɫֵ
	if (useTexture == true)
	{
		texColor = g_tex.Sample(samplerTex, pin.tex);
	}
	float4 litColor = texColor;

	//��ʼ���ռ���
	if (numLights > 0)
	{
		float3 toEye = normalize(g_eyePos - pin.posTrans);
		float3 normal = normalize(pin.normal);

		float A = float4(0.f, 0.f, 0.f, 0.f);
		float D = float4(0.f, 0.f, 0.f, 0.f);
		float S = float4(0.f, 0.f, 0.f, 0.f);

		[unroll]
		for (int i = 0; i < numLights; ++i)
		{
			float4 ambient, diffuse, specular;
			ComputeDirectionalLight(g_material, g_lights[i], normal,toEye,ambient, diffuse, specular);
			A += ambient;
			D += diffuse;
			S += specular;
		}
		//����+���ռ��㹫ʽ�� ����*(������+�������)+�߹�
		litColor = texColor * (A + D) + S;
	}
	//͸���� ����͸����*����������͸����
	litColor.a = g_material.diffuse.a * texColor.a;

	return litColor;
}

//�����Ƿ�ʹ�������Լ���Դ���������ֲ�ͬ��technique
technique11 Light1
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(1, false)));
	}
}

technique11 Light2
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(2, false)));
	}
}

technique11 Light3
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(3, false)));
	}
}

technique11 Light1Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(1, true)));
	}
}

technique11 Light2Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(2, true)));
	}
}

technique11 Light3Tex
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetPixelShader(CompileShader(ps_5_0, PS(3, true)));
	}
}