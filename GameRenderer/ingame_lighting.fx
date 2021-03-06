//for common
struct VS_IN
{
	float4 vPosition:POSITION;
	float2 vUV: TEXCOORD0;
};
struct VS_OUT
{
	float4 vPosition:POSITION;
	float4 vClipPosition:TEXCOORD0;
	float2 vUV: TEXCOORD1;
};
struct PS_IN
{
	float4 vClipPosition:TEXCOORD0;
	float2 vUV: TEXCOORD1;
};
struct PS_OUT
{
	float4 vColor: COLOR0;
	float4 vSpecular:COLOR1;
};
VS_OUT vs_main(VS_IN input)
{
	VS_OUT output;
	output.vPosition = input.vPosition;
	output.vPosition.w = 1.f;
	output.vClipPosition = output.vPosition;
	output.vUV = input.vUV;
	return output;
}
texture g_normalMap;
texture g_specularMap;
texture g_depthMap;
sampler NormalMapSampler = sampler_state
{
	texture = g_normalMap;
};
sampler DepthMapSampler = sampler_state
{
	texture = g_depthMap;
	minfilter = linear;
	magfilter = linear;
};

sampler SpecularMapSampler = sampler_state
{
	texture = g_specularMap;
	minfilter = linear;
	magfilter = linear;
};
matrix g_mInverseViewProj;
float4 g_vLightDirectionAndPower;
float4 g_vLightDiffuse;
float4 g_vLightAmbient;
float4 g_vCameraPosition;
PS_OUT ps_directional_light(PS_IN input)
{
	PS_OUT output;
	output.vSpecular = float4(0.f, 0.f, 0.f, 1.f);
	float4 vSpecular = tex2D(SpecularMapSampler, input.vUV);
	float4 fPower = vSpecular.w;
	vSpecular.w = 1.f;

	float4 vNormalFactor = tex2D(NormalMapSampler, input.vUV);
	//float2 fenc = vNormalFactor.xy * 4.f - 2.f;
	//float f = dot(fenc, fenc);
	//float g = sqrt(1.f - f / 4.f);
	//float3 vNormal = normalize(float3(fenc * g, 1.f - f / 2.f));
	//float depth = vNormalFactor.z;
	//float z = vNormalFactor.w;

	float depth = tex2D(DepthMapSampler, input.vUV).r;
	float z = tex2D(DepthMapSampler, input.vUV).g;
	float3 vNormal = normalize(vNormalFactor.xyz * 2.f - 1.f);

	float4 vPosition = mul(float4(input.vClipPosition.xy, depth, 1.f) * z, g_mInverseViewProj);
	float4 vLightDir = normalize(float4(g_vLightDirectionAndPower.xyz, 0.f));
	float intensity = saturate(dot(vLightDir * -1, vNormal));
	float4 vDiffuse = intensity * g_vLightDiffuse + g_vLightAmbient;
	vDiffuse.a = intensity;
	if (intensity > 0.f)
	{
		float3 vReflect = reflect(vLightDir.xyz, vNormal.xyz).xyz;
		float3 vLook = normalize(vPosition.xyz - g_vCameraPosition.xyz);
		float power = pow(saturate(dot(vReflect, -vLook)), fPower);
		output.vSpecular = (float4) power;
		output.vSpecular = output.vSpecular * g_vLightDiffuse * vSpecular;
		output.vSpecular.a = 1.f;
	}
	output.vColor = vDiffuse;
	return output;
}


technique Default_Device
{
	pass directionalLight
	{
		ZEnable = false;
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_directional_light();
	}
}