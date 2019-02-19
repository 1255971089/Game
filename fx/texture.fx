cbuffer cbPerObject
{
	float4x4 g_worldViewProj;
};

//纹理
Texture2D g_tex;

//设置过滤方式
SamplerState samTex
{
	Filter = MIN_MAG_MIP_LINEAR;
};

struct VertexIn
{
	float3 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VertexOut
{
	float4 posH  : SV_POSITION;
	float2 tex : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.posH = mul(float4(vin.pos, 1.0f), g_worldViewProj);
	vout.tex = vin.tex;
	return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
	float4 texColor = g_tex.Sample(samTex,pin.tex);
	return texColor;
}

technique11 TextureTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
}
