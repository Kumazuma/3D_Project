#pragma once
#include"RenderObject.h"
#include"RenderEntity.h"

#include "MeshObject.h"
#include <vector>
#include <memory>
#include <unordered_map>
#include <set>
#include "SpinLock.hpp"
#include <tuple>
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
	auto PrepareRender(IRenderer* pRenderModule)->void override;
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void;
	auto Clone()const->RenderObject*;
	auto FindFrameTransfromByName(std::wstring const& frameName, DirectX::XMFLOAT4X4* const pOut)->HRESULT;
	auto FindFrameTransfromByName(std::wstring const& frameName)->DirectX::XMFLOAT4X4 const*;

	auto IsAnimationSetEnd()->bool;
	auto SetAnimationSet(u32 idx, bool repeat = true)->void;
	auto PlayAnimation(f32 timeDelta)->void;
	auto GetAnimationCount()const->u32;
	auto GetFrameNamesRef()const->std::set<std::wstring> const&;
	auto GetCurrentAnimSetLength()const->f32;
	auto GetCurrentSeek()const->f32;
protected:
	SkinnedXMeshObject();
	SkinnedXMeshObject(SkinnedXMeshObject const* rhs);
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
	auto InitializeFrameMatrix(Frame* pFrame)->void;
	auto UpdateFrameMatrices(Frame* pFrame, DirectX::XMFLOAT4X4 const& mat)->void;
private:
	D3DXFRAME* m_pRootFrame;
	bool m_repeat;

	//std::unordered_map < CustomMeshContainer*, COMPtr<IDirect3DVertexDeclaration9> > m_vertexDecls;
	//std::unordered_map<CustomMeshContainer*, COMPtr<IDirect3DVertexBuffer9> > m_boneInfoBuffers;
	std::unordered_map<CustomMeshContainer*, std::vector< DirectX::XMFLOAT4X4>> m_renderedMatrices;
	std::unordered_map<std::wstring, DirectX::XMFLOAT4X4> m_combinedOffsetMatrices;
	std::shared_ptr<std::set<std::wstring> > m_pFrameNames;
	std::shared_ptr<HierarchyLoader> m_pHierarchyLoader;
	std::shared_ptr<SkinnedMeshEntity> m_entity;
	std::shared_ptr<SpinLock> m_spinLock;
	std::unique_ptr<AnimationController> m_pAnimCtrler;
	std::vector<CustomMeshContainer* > m_meshContainters;
};
struct SkinnedXMeshObject::Frame : public D3DXFRAME
{
	std::shared_ptr<DirectX::XMFLOAT4X4> combinedTransformationMatrix;
	std::wstring name;
};
struct SkinnedXMeshObject::CustomMeshContainer : public D3DXMESHCONTAINER
{
	std::vector<COMPtr<IDirect3DTexture9>> textures;
	COMPtr<ID3DXMesh> pOriginalMesh;
	DWORD fvf;
	u32 boneCount;
	u32 vertexSize;
	u32 vertexCount;
	std::vector< DirectX::XMFLOAT4X4> frameOffsetMatries;
	std::vector<std::shared_ptr<DirectX::XMFLOAT4X4> > frameCombinedMatries;
	std::vector<D3DXMATERIAL> materials;
	std::vector<DWORD> adjacency;
	//첫번째는 시작구간, 두번째는 갯수
	std::vector< std::tuple<u32, u32> > subsetTriangleInfo;
};
class SkinnedMeshEntity : public RenderEntity
{
public:
	SkinnedMeshEntity(SkinnedXMeshObject* pObj);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void;
private:
	SkinnedXMeshObject* m_pObj;
};
#include"XMeshHierarchyLoader.h"
#include"XMeshAnimCtrler.h"