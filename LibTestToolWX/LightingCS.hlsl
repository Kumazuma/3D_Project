cbuffer c1 : register(b0)
{
	int g_lightType;
	float3 g_vLightDirection;
	float4 g_vLightDiffuse;
	float4 g_vLightAmbient;
}
cbuffer c2 : register(b1)
{
	float4x4 g_mInverseViewProj;
	float4 g_vCameraPosition;
	uint2 g_bufferSize;
	uint2 g_padding;
}
SamplerState SampleType;
Texture2D gTexture:register(t0);
Texture2D gLightMap:register(t1);
RWTexture2D<float4> gNormalMap:register(u0);
RWTexture2D<float2> gDepthMap:register(u1);
RWTexture2D<float4> gSharpnessMap:register(u2);

RWTexture2D<float4> gLightAmbientMap:register(u3);
RWTexture2D<float4> gLightSpecularMap:register(u4);

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (DTid.x >= g_bufferSize.x || DTid.y >= g_bufferSize.y)
	{
		return;
	}
	//float2 tex = DTid.xy / (float2) gBufferSize.xy;
	//float3 color = gTexture.SampleLevel(SampleType, tex, 0).rgb;
	//float3 light = gLightMap.SampleLevel(SampleType, tex, 0).rgb;
	//float3 finalColor = color * gAmbientColor * gAmbientPower;

	float2 depth = gDepthMap[DTid.xy];
	
	float4 vNormal = gNormalMap[DTid.xy];
	vNormal.w = 0.f;
	vNormal.xyz = normalize(vNormal.xyz * 2.f - 1.f);
	float4 sharpness = gSharpnessMap[DTid.xy];
	float power = sharpness.w;
	sharpness.w = 1.f;
	float4 vPosition = float4(DTid.xy / (float2)g_bufferSize.xy, 0.f, 0.f);
	vPosition *= 2.f;
	vPosition.y *= -1.f;
	vPosition.zw = float2(depth.x, 1.f);
	vPosition *= depth.y;
	vPosition = mul(vPosition, g_mInverseViewProj);

	float intensity = saturate(dot(g_vLightDirection * -1, vNormal.xyz));
	float4 vDiffuse = intensity * g_vLightDiffuse + g_vLightAmbient;
	vDiffuse.a = intensity;

	float4 vOtherAmbient = gLightAmbientMap[DTid.xy];
	vDiffuse += vOtherAmbient;
	vDiffuse = saturate(vDiffuse);
	gLightAmbientMap[DTid.xy] = vDiffuse;

	if (intensity > 0.f)
	{
		float3 vReflect = reflect(g_vLightDirection.xyz, vNormal.xyz).xyz;
		float3 vLook = normalize(vPosition.xyz - g_vCameraPosition.xyz);
		float power = pow(saturate(dot(vReflect, -vLook)), sharpness.a);
		float4 vSpecular = (float4)power;
		vSpecular = vSpecular * g_vLightDiffuse * sharpness;
		vSpecular.a == 1.f;
		float4 vOtherSpecular = gLightSpecularMap[DTid.xy];
		vSpecular += vOtherSpecular;
		vSpecular = saturate(vSpecular);
		gLightSpecularMap[DTid.xy] = vSpecular;
	}
}