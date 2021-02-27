cbuffer ToneMapping
{
	float g_whitePoint:register(c0);
	uint2 g_bufferSize:register(c80);
};
RWTexture2D<float4> hdrRenderTargetTexture;
RWTexture2D<unorm float4> sdrRenderTargetTexture;

[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	if (g_bufferSize.x >= DTid.x || g_bufferSize.y >= DTid.y)
	{
		return;
	}
	float4 hdr = hdrRenderTargetTexture[DTid.xy];
	float L = hdr.a;
	float Cb = hdr.b;
	float Cr = hdr.r;
	L = L * (1 + (L / (g_whitePoint * g_whitePoint))) / (1 + L);
	float4 ldr = (float4)1.f;
	ldr.r = L + 1.403f * Cr;
	ldr.g = L - 0.344f * Cb - 0.714f * Cr;
	ldr.b = L + 1.770f * Cb;
	ldr = pow(ldr, 1.f / 2.2f);
	ldr.a = 1.f;
	sdrRenderTargetTexture[DTid.xy] = ldr;
}