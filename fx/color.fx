cbuffer cbPerObject
{
	float4x4 g_worldviewproj;
};

struct VertexIn
{
	float3 PosL:POSITION;
	float4 Color:COLOR;
}; 

struct VertexOut
{
	float4 PosH:SV_POSITION;
	float4 Color:COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	vout.PosH = mul(float4(vin.PosL, 1.0f), g_worldviewproj);
	vout.Color = vin.Color;
	return vout;
}

float4 PS(VertexOut pin):SV_TARGET
{
	return pin.Color;
}

technique11 ColorTech
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, PS()));
	}
};