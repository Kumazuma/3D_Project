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
	float4 ldr = hdr * (1 + (hdr / (g_whitePoint * g_whitePoint))) / (1 + hdr);
	ldr = pow(ldr, 1.f / 2.2f);
	ldr.a = 1.f;
	sdrRenderTargetTexture[DTid.xy] = ldr;

}