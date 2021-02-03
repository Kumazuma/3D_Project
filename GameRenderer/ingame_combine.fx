//for common
struct VS_IN
{
	float4 vPosition:POSITION;
	float2 vUV: TEXCOORD0;
};
struct VS_OUT
{
	float4 vPosition:POSITION;
	float2 vUV: TEXCOORD0;
};
struct PS_IN
{
	float2 vUV: TEXCOORD0;
};
struct PS_OUT
{
	float4 vColor: COLOR0;
};
VS_OUT vs_main(VS_IN input)
{
	VS_OUT output;
	output.vPosition = input.vPosition;
	output.vPosition.w = 1.f;
	output.vUV = input.vUV;
	return output;
}
texture g_shadeMap;
texture g_albedoMap;
texture g_specularMap;
texture g_depthMap;
sampler ShadeMapSampler = sampler_state
{
	texture = g_shadeMap;
	minfilter = linear;
	magfilter = linear;
};
sampler SpecularMapSampler = sampler_state
{
	texture = g_specularMap;
	minfilter = linear;
	magfilter = linear;
};
sampler AlbedoMapSampler = sampler_state
{
	texture = g_albedoMap;
	minfilter = point;
	magfilter = point;
};
sampler DepthMapSampler = sampler_state
{
	texture = g_depthMap;
	minfilter = linear;
	magfilter = linear;
};
float g_farZ;
float4 ps_main(PS_IN input) :COLOR0
{
	float4 vShade = tex2D(ShadeMapSampler, input.vUV);
	float4 vAlbedo = tex2D(AlbedoMapSampler, input.vUV);
	float4 vSpecular = tex2D(SpecularMapSampler, input.vUV);
	float4 vColor = vSpecular + vAlbedo * vShade;
	float z = tex2D(DepthMapSampler, input.vUV).g / g_farZ;

	float fogFactor = 1.0f / pow(2.71828f, pow(z * 0.66f, 3) );
	//finalColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
	vColor = fogFactor * vColor + (1.f - fogFactor);
	vColor.a = vAlbedo.a;
	return vColor;
}

technique Default_Device
{
	pass combine
	{
		ZWriteEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = srcalpha;
		DestBlend = invsrcalpha;
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}
}