uint2 g_renderSize;

texture g_diffuseTexture;
sampler g_sampler;
struct PSIn
{
	float2 tex:TEXCOORD0;
};
float4 main(PSIn pixel) : COLOR
{
	float4 color = (float4)0;
	return color;
}