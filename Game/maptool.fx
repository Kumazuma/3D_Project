//for common
matrix g_mViewProj;
matrix g_mView;
matrix g_mWorld;
matrix g_mNormalWorld;
texture g_diffuseTexture;
sampler DiffuseTextureSampler = sampler_state
{
	texture = g_diffuseTexture;
	minfilter = linear;
	magfilter = linear;
	addressU = wrap;
	addressV = wrap;
};
//for no specularmap
float4 g_vSpecular;
struct NOSPECULARMAP_VS_IN
{
	float4 vPosition:POSITION;
	float4 vNormal:NORMAL;
	float2 vUV:TEXCOORD0;
};
struct NOSPECULARMAP_VS_OUT
{
	float4 vPosition:POSITION;
	float4 vNormal:NORMAL0;
	float2 vUV:TEXCOORD1;
	float4 vClipPosition:TEXCOORD2;
};
struct NOSPECULARMAP_PS_IN
{
	float4 vNormal:NORMAL0;
	float2 vUV:TEXCOORD1;
	float4 vClipPosition:TEXCOORD2;
};
struct NOSPECULARMAP_PS_OUT
{
	float4 diffuse:COLOR0;
	float4 normal:COLOR1;
	float4 specular:COLOR2;
	float4 depth:COLOR3;
};
NOSPECULARMAP_VS_OUT VS_NOSCPECULARMAP(NOSPECULARMAP_VS_IN input)
{
	NOSPECULARMAP_VS_OUT output;
	float4 vPosition = float4(input.vPosition.xyz, 1.f);
	vPosition = mul(vPosition, g_mWorld);
	vPosition = mul(vPosition, g_mViewProj);
	output.vPosition = vPosition;
	output.vClipPosition = vPosition;
	output.vNormal = mul(float4(input.vNormal.xyz, 0.f), g_mNormalWorld);

	output.vUV = input.vUV;
	return output;
}
NOSPECULARMAP_PS_OUT PS_NOSCPECULARMAP(NOSPECULARMAP_PS_IN input)
{
	NOSPECULARMAP_PS_OUT output;
	output.diffuse = tex2D(DiffuseTextureSampler, input.vUV);
	output.specular = g_vSpecular;
	output.normal = input.vNormal * 0.5f + 0.5f;
	output.normal.w = 1.f;
	output.depth = (float4)input.vClipPosition.z / input.vClipPosition.w;
	return output;
}

technique Default_Device
{
	pass NONALPHA_NO_SPECULARMAP
	{
		vertexshader = compile vs_3_0 VS_NOSCPECULARMAP();
		pixelshader = compile ps_3_0 PS_NOSCPECULARMAP();
	}
	pass COLLIDER_PIPELINE
	{
		alphablendenable = true;
		fillmode = wireframe;
		cullmode = none;
		vertexshader = compile vs_3_0 VS_NOSCPECULARMAP();
		pixelshader = compile ps_3_0 PS_NOSCPECULARMAP();
	}
};