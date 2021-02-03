struct Effect_VSIn
{
	float4 vPosition:POSITION;
	float2 vUV:TEXCOORD0;
};
struct Effect01_VSOut
{
	float4 vPosition:POSITION;
	float4 vClipPosition:TEXCOORD1;
	float2 vUV:TEXCOORD0;
};
struct Effect01_PSIn
{
	float4 vClipPosition:TEXCOORD1;
	float2 vUV:TEXCOORD0;
	//float2 vUV2:TEXCOORD1;
};
//for common
float4x4 g_mViewProj;
float4x4 g_mView;
float4x4 g_mWorld;

float4x4 g_mUVTransform1;
float4x4 g_mUVTransform2;

texture g_effectTexture1;
texture g_effectTexture2;

texture g_depthMap;
Effect01_VSOut VS_Effect01(Effect_VSIn input)
{
	Effect01_VSOut output;
	float4 vPosition = float4(input.vPosition.xyz, 1.f);
	vPosition = mul(vPosition, g_mWorld);
	vPosition = mul(vPosition, g_mViewProj);
	output.vPosition = vPosition;
	output.vUV = mul(float4(input.vUV, 0.0f, 1.0f), g_mUVTransform1).xy;
	output.vClipPosition = vPosition;
	//output.vUV2 = mul(float4(input.vUV, 0.0f, 1.0f), g_mUVTransform2).xy;
	return output;
}

sampler Effect1TextureSampler = sampler_state
{
	texture = g_effectTexture1;
	minfilter = linear;
	magfilter = linear;
	addressU = wrap;
	addressV = wrap;
};
sampler DepthMapTextureSampler = sampler_state
{
	texture = g_depthMap;
	minfilter = linear;
	magfilter = linear;
	addressU = wrap;
	addressV = wrap;
};
sampler Effect2TextureSampler = sampler_state
{
	texture = g_effectTexture2;
	minfilter = linear;
	magfilter = linear;
	addressU = wrap;
	addressV = wrap;
};
float4 PS_Effect01(Effect01_PSIn input) :COLOR0
{
	float4 diffuse1 = tex2D(Effect1TextureSampler, input.vUV);
	float2 depthUV = input.vClipPosition.xy;
	depthUV.y *= -1.f;
	depthUV = depthUV * 0.5f + 0.5f;
	float2 depth = tex2D(DepthMapTextureSampler, depthUV).rg;
	float z = input.vClipPosition.z/input.vClipPosition.w;
	//diffuse1.a *= saturate(depth.x/ z );
	//float4 diffuse2 = tex2D(Effect2TextureSampler, input.vUV2);

	//diffuse2 = diffuse2 ;
	//diffuse2.a = diffuse1.a;
	return diffuse1;
}
float4 VS_Effect02(Effect_VSIn input) :POSITION
{
	float4 vPosition = float4(input.vPosition.xyz, 1.f);
	vPosition = mul(vPosition, g_mWorld);

	return vPosition;
}
float4 g_effect2Color;
float4 PS_Effect02(void) : COLOR
{
	return g_effect2Color;
}
technique Default_Device
{
	pass Effect1
	{
		AlphaBlendEnable = true;
		SrcBlend = srcalpha;
		DestBlend = invsrcalpha;
		CullMode = NONE;
		vertexshader = compile vs_3_0 VS_Effect01();
		pixelshader = compile ps_3_0 PS_Effect01();
	}
	pass Effect2
	{
		AlphaBlendEnable = true;
		SrcBlend = srcalpha;
		DestBlend = invsrcalpha;
		CullMode = NONE;
		vertexshader = compile vs_3_0 VS_Effect02();
		pixelshader = compile ps_3_0 PS_Effect02();
	}
};