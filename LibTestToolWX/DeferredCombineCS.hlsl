
RWTexture2D<unorm float4> gAlbedoMap:register(u0);
RWTexture2D<float4> gLightAmbientMap:register(u1);
RWTexture2D<float4> gLightSpecularMap:register(u2);
RWTexture2D<float4> gRenderTarget:register(u3);


[numthreads(1, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	float4 vAlbedo = gAlbedoMap[DTid.xy];
	float4 vAmbient = gLightAmbientMap[DTid.xy];
	float4 vSpecular = gLightSpecularMap[DTid.xy];
	if (vAlbedo.a <= 0.1f)
	{
		return;
	}
	float4 vColor = vSpecular + pow( vAlbedo, 2.2f) * vAmbient;
	float3 yFactor = float3(0.299f, 0.587f, 0.114f);

	float Y = dot(yFactor, vColor.rgb);
	float Cb = (vColor.b - Y) * 0.565f;
	float Cr = (vColor.r - Y) * 0.713f;
	gRenderTarget[DTid.xy] = float4(Cr, 0, Cb, Y);
}