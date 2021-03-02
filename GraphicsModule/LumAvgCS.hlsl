cbuffer c2 : register(b0)
{
	uint2 sourceTextureSize;
	uint2 sourceTextureRange;
	uint2 destTextureRange;
}

Texture2D			sourceTexture:register(t0);
SamplerState SampleType;
RWTexture2D<float4> destTexture:register(u0);
[numthreads(1, 1, 1)]
void main(
	uint3 DTid : SV_DispatchThreadID,
	uint uid : SV_GroupIndex)
{
	float2 centerUV = ((float2)DTid.xy) / destTextureRange.xy;
	float4 YCbCr = (float4)0;
	float c = 0.f;
	float2 ratio = (float2)sourceTextureRange/ sourceTextureSize;
	for (int y = -1; y < 1; ++y)
	{
		for (int x = -1; x < 1; ++x)
		{
			float2 uv = centerUV;
			uv.x += x / (float)sourceTextureRange.x;
			uv.y += y / (float)sourceTextureRange.y;
			if (
				0.f <= uv.x && uv.x <= 1.f &&
				0.f <= uv.y && uv.y <= 1.f 
				)
			{
				YCbCr += sourceTexture.SampleLevel(SampleType, uv * ratio, 0);
				++c;
			}
		}
	}
	destTexture[DTid.xy] = YCbCr / c;
}