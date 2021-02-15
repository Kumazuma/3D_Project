cbuffer WorldCBuffer : register(b0)
{
	float4x4 g_mViewProj;
}
cbuffer ObjectCBuffer : register(b1)
{
	float4x4 g_mWorld;
}

struct Input
{
	float4 vPosition:POSITION0;
	float3 vNormal:NORMAL0;
	float2 vTex:TEXCOORD0;
};
struct Output
{
	float4 vPosition:SV_POSITION;
	float3 vNormal:NORMAL0;
	float2 vTex:TEXCOORD0;
};

Output main(Input input) 
{
	Output output;
	float4 vPosition = float4(input.vPosition.xyz, 1.f);
	float4 vNormal = float4(input.vNormal.xyz, 0.f);
	output.vPosition = mul(mul(vPosition, g_mWorld), g_mViewProj);
	output.vNormal = mul(vNormal, g_mWorld).xyz;
	output.vTex = input.vTex;
	return output;
}