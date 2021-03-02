cbuffer WorldCBuffer : register(b0)
{
	float4x4 g_mViewProj;
}
cbuffer ObjectCBuffer : register(b1)
{
	float4x4 g_mWorld;
	float4x4 g_mNormalWorld;
}
cbuffer AnimationBuffer : register(b2)
{
	float4x4 g_mBones[256];
}

struct Input
{
	float4 vPosition:POSITION0;
	float3 vNormal:NORMAL0;
	float2 vTex:TEXCOORD0;
	uint4  boneIndices:BLENDINDICES;
	float4 boneWeights:BLENDWEIGHT;
};
struct Output
{
	float4 vPosition:SV_POSITION;
	float3 vNormal:NORMAL0;
	float2 vTex:TEXCOORD0;
};

Output main(Input vertex)
{
	Output output = (Output)0;
	float4 vPos = vertex.vPosition;
	uint4  indices = vertex.boneIndices;
	uint4  weights = vertex.boneWeights;
	vPos.w = 1.f;
	//float4x4 mAnim;
	//mAnim =
	//	g_mBones[indices.x] * weights.x +
	//	g_mBones[indices.y] * weights.y +
	//	g_mBones[indices.z] * weights.z +
	//	g_mBones[indices.w] * weights.w;
	//vPos = mul(mAnim, vPos);
	vPos = mul(g_mWorld, vPos);
	vPos = mul(g_mViewProj, vPos);

	float4 vNormal = float4(vertex.vNormal, 0.f);
	//vNormal = mul(mAnim, vNormal);
	vNormal = mul(g_mNormalWorld, vNormal);

	output.vPosition = vPos;
	output.vNormal = vNormal.xyz;
	output.vTex = vertex.vTex;
	return output;
}