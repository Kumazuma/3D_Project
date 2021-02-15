cbuffer c1 : register(b0)
{
	int lightType;
	float3 vDirection;
	float4 vDiffuse;
	float4 vAmbient;
	uint2 gBufferSize;
	uint2 padding;
}
SamplerState SampleType;
Texture2D gTexture:register(t0);
Texture2D gLightMap:register(t1);
RWTexture2D<float4> gNormalMap:register(u0);
RWTexture2D<float2> gDepthMap:register(u1);
RWTexture2D<float4> gResult:register(u4);
[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= gBufferSize.x || DTid.y >= gBufferSize.y)
	{
		return;
	}
	//float2 tex = DTid.xy / (float2) gBufferSize.xy;
	//float3 color = gTexture.SampleLevel(SampleType, tex, 0).rgb;
	//float3 light = gLightMap.SampleLevel(SampleType, tex, 0).rgb;
	//float3 finalColor = color * gAmbientColor * gAmbientPower;
	float4 light1 = gResult[DTid.xy];
	float2 depth = gDepthMap[DTid.xy];
	float4 r = float4(1.f, 1.f, 1.f, 1.f);
	light1 = light1 + r;
	gResult[DTid.xy] = light1;
}