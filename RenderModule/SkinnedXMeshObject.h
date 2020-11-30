#pragma once
#include"RenderObject.h"
#include "RenderModule.h"
#include "MeshObject.h"
#include <vector>
#include <memory>
#include <unordered_map>
class SkinnedMeshEntity;
class DLL_CLASS SkinnedXMeshObject : public XMeshObject
{
	class HierarchyLoader;
	class AnimationController;
	struct CustomMeshContainer;
	friend class SkinnedMeshEntity;
public:
	struct Frame;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, SkinnedXMeshObject** pOut)->HRESULT;
	auto PrepareRender(RenderModule* pRenderModule)->void;
	auto Render(RenderModule* pRenderModule)->void;
	auto Clone()const->RenderObject*;
	auto FindFrameTransfromByName(std::string const& frameName, DirectX::XMFLOAT4X4* const pOut)->HRESULT;
	auto IsAnimationSetEnd()->bool;
	auto SetAnimationSet(u32 idx)->void;
	auto PlayAnimation(f32 timeDelta)->void;
	auto GetAnimationCount()const->u32;
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
	std::shared_ptr<SkinnedMeshEntity> m_entity;
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
class SkinnedMeshEntity : public RenderEntity
{
public:
	SkinnedMeshEntity(SkinnedXMeshObject* pObj);
	auto Render(RenderModule* pRenderModule)->void;
private:
	SkinnedXMeshObject* m_pObj;
};
#include"XMeshHierarchyLoader.h"
#include"XMeshAnimCtrler.h"