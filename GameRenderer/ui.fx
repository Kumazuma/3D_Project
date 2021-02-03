struct UIIn
{
	float4 vPosition:POSITION;
	float2 vUV:TEXCOORD0;
};
struct UI01_VSOut
{
	float4 vPosition:POSITION;
	float2 vUV:TEXCOORD0;
};
struct UI01_PSIn
{
	float2 vUV:TEXCOORD0;
	float2 vUV2:TEXCOORD1;
};
//for common
float4x4 g_mViewProj;
float4x4 g_mProj;
float4x4 g_mView;
float4x4 g_mWorld;

float4x4 g_mUVTransform1;

texture g_uiTexture1;

UI01_VSOut VS_UI01(UIIn input)
{
	UI01_VSOut output;
	float4 vPosition = float4(input.vPosition.xyz, 1.f);
	vPosition = mul(vPosition, g_mWorld);
	vPosition = mul(vPosition, g_mProj);
	output.vPosition = vPosition;
	output.vUV = mul(float4(input.vUV, 0.0f, 1.0f), g_mUVTransform1).xy;
	return output;
}

sampler UITextureSampler = sampler_state
{
	texture = g_uiTexture1;
	minfilter = linear;
	magfilter = linear;
	addressU = wrap;
	addressV = wrap;
};
float4 PS_UI01(UI01_PSIn input) :COLOR0
{
	float4 diffuse1 = tex2D(UITextureSampler, input.vUV);
	return diffuse1;
}
technique Default_Device
{
	pass UI1
	{
		ZEnable = false;
		AlphaBlendEnable = true;
		SrcBlend = srcalpha;
		DestBlend = invsrcalpha;
		CullMode = NONE;
		vertexshader = compile vs_3_0 VS_UI01();
		pixelshader = compile ps_3_0 PS_UI01();
	}
};