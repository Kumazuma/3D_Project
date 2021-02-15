cbuffer Material
{
	float4 sharpness;
};
struct PSInput
{
	float4 vPosition:SV_POSITION;
	float3 vNormal:NORMAL0;
	float2 vTex:TEXCOORD0;
};
struct PSOutput
{
	float4 albedo:SV_TARGET0;
	float4 normal:SV_TARGET1;
	float4 sharpness:SV_TARGET2;
	float2 depth:SV_TARGEt3;
};

Texture2D gTexture;
SamplerState SampleType;

PSOutput main(PSInput input)
{
	float4 vPosition = input.vPosition;
	float depth = vPosition.z / vPosition.w;
	float viewSpaceZ = vPosition.w;
	PSOutput output;
	output.albedo = gTexture.Sample(SampleType, input.vTex);
	output.normal = float4(input.vNormal * 0.5f + 0.5f, 0.f);
	output.sharpness = sharpness;
	output.depth = float2(depth, viewSpaceZ);
	return output;
}