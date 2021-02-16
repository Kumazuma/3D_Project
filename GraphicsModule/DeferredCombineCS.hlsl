
RWTexture2D<unorm float4> gAlbedoMap:register(u0);
RWTexture2D<float4> gLightAmbientMap:register(u1);
RWTexture2D<float4> gLightSpecularMap:register(u2);
RWTexture2D<unorm float4> gRenderTarget:register(u3);


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
	float4 vColor = vSpecular + vAlbedo * vAmbient;
	gRenderTarget[DTid.xy] = vColor;
}