uint2 g_renderSize;

texture g_diffuseTexture;
sampler g_sampler;
struct PSIn
{
	float2 tex:TEXCOORD0;
};
float4 main(PSIn pixel) : COLOR
{
	float4 color = g_diffuseTexture[(uint2)(pixel.tex * g_renderSize)];
	return color;
}