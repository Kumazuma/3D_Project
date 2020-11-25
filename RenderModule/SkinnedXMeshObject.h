#pragma once
#include"RenderObject.h"
#include "MeshObject.h"
#include <vector>
#include <memory>
#include <unordered_map>
class DLL_CLASS SkinnedXMeshObject : public XMeshObject
{
	class HierarchyLoader;
	class AnimationController;
	struct CustomMeshContainer;
public:
	struct Frame;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, SkinnedXMeshObject** pOut)->HRESULT;
	auto Render(RenderModule* pRenderModule)->void;
	auto Clone()const->RenderObject*;
	auto FindFrameTransfromByName(std::string const& frameName, DirectX::XMFLOAT4X4* const pOut)->HRESULT;
	auto IsAnimationSetEnd()->bool;
	auto SetAnimationSet(u32 idx)->void;
	auto PlayAnimation(f32 timeDelta)->void;
protected:
	SkinnedXMeshObject();
	SkinnedXMeshObject(SkinnedXMeshObject const* rhs);
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
	auto InitializeFrameMatrix(Frame* pFrame)->void;
	auto UpdateFrameMatrices(Frame* pFrame, DirectX::XMFLOAT4X4 const& mat)->void;
private:
	D3DXFRAME* m_pRootFrame;
	std::shared_ptr<HierarchyLoader> m_pHierarchyLoader;
	std::unique_ptr<AnimationController> m_pAnimCtrler;
	std::vector<CustomMeshContainer* > m_meshContainters;
	std::unordered_map<std::string, DirectX::XMFLOAT4X4> m_renderedMatrices;
};
struct SkinnedXMeshObject::Frame : public D3DXFRAME
{
	std::shared_ptr<DirectX::XMFLOAT4X4> combinedTransformationMatrix;
};
struct SkinnedXMeshObject::CustomMeshContainer : public D3DXMESHCONTAINER
{
	std::vector<COMPtr<IDirect3DTexture9>> textures;
	COMPtr<ID3DXMesh> pOriginalMesh;
	u32 boneCount;

	std::vector< DirectX::XMFLOAT4X4> frameOffsetMatries;
	std::vector<std::shared_ptr<DirectX::XMFLOAT4X4> > frameCombinedMatries;
	std::vector< DirectX::XMFLOAT4X4> renderingMatries;
	std::vector<D3DXMATERIAL> materials;
	std::vector<DWORD> adjacency;
};
#include"XMeshHierarchyLoader.h"
#include"XMeshAnimCtrler.h"