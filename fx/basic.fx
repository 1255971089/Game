#include "LightHelper.fx"

//针对每个物体
cbuffer PerObject
{
	float4x4 g_worldViewProj;       //世界 视角 投影矩阵
	float4x4 g_world;               //世界矩阵
	float4x4 g_worldInvTranspose;   //世界矩阵逆矩阵的转置，用于调整法线
	Material g_material;            //材质
	float4x4 g_texTrans;            //纹理变换
};

//针对每一帧
cbuffer PerFrame
{
	DirectionalLight g_lights[3]; //光源数组
	float3 g_eyePos; //观察点
};

//纹理
Texture2D g_tex;

SamplerState samplerTex
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexIn
{
	float3 pos      : POSITION;     //顶点坐标
	float3 normal   : NORMAL;        //顶点法线
	float2 tex      : TEXCOORD;      //纹理坐标
};

struct VertexOut
{
	float3 posTrans : POSITION; //世界变换后的坐标，乘以world矩阵
	float4 posH : SV_Position;   //投影后的坐标
	float3 normal : NORMAL;      //世界变换后的顶点法线
	float2 tex : TEXCOORD;       //转换后的纹理坐标
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	//世界变换后的顶点坐标
	vout.posTrans = mul(float4(vin.pos, 1.0f), g_world).xyz;
	//投影变换后的顶点坐标
	vout.posH = mul(float4(vin.pos, 1.0f), g_worldViewProj);
	//变换后的法线坐标
	vout.normal = mul(float4(vin.normal, 1.0f), g_worldInvTranspose).xyz;
	//变换后的纹理坐标
	vout.tex = mul(float4(vin.tex, 0.0f, 1.0f), g_texTrans).xy;

	return vout;
}

float4 PS(VertexOut pin, uniform int numLights, uniform bool useTexture) :SV_Target
{
	//初始颜色为白色
	float4 texColor = float4(1.f, 1.f, 1.f, 1.f);
	//使用纹理则获得相应颜色值
	if (useTexture == true)
	{
		texColor = g_tex.Sample(samplerTex, pin.tex);
	}
	float4 litColor = texColor;

	//开始光照计算
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
		//纹理+光照计算公式： 纹理*(环境光+漫反射光)+高光
		litColor = texColor * (A + D) + S;
	}
	//透明度 纹理透明度*材质漫反射透明度
	litColor.a = g_material.diffuse.a * texColor.a;

	return litColor;
}

//根据是否使用纹理，以及光源个数，划分不同的technique
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