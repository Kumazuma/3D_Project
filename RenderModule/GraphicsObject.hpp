#pragma once
#include"resource.hpp"
#include<DirectXMath.h>
#include<string>
#include<unordered_map>
class GraphicsObject
{
public:
	virtual ~GraphicsObject() = default;
	virtual auto Render()->void = 0;
	virtual auto SetTransform(DirectX::XMFLOAT4X4 const* t)->void = 0;
	virtual auto GetTransform(DirectX::XMFLOAT4X4* t)const->void = 0;

private:
};
enum class MeshRenderType {
	DeferredDefault = 0,
	DeferredAlphaTest = 1,
	ForwardDefault = 2
};

class MeshGraphicsObject
{
public:
	auto IsCanRaycast()const->bool;
	auto Raycast(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDir, float* pOut)->bool;
	auto SetSubsetRenderType(std::wstring const& subsetName, MeshRenderType renderType)->bool;
	auto GetSubsetRenderType(std::wstring const& subsetName, MeshRenderType* out)->bool;
private:
	
};