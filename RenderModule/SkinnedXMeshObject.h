#pragma once
#include"RenderObject.h"
#include "MeshObject.h"
#include <vector>
#include <memory>
class DLL_CLASS SkinnedXMeshObject : public XMeshObject
{
	class HierarchyLoader;
	class AnimationController;
	struct CustomMeshContainer;
	struct Frame;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& filePath, SkinnedXMeshObject** pOut)->HRESULT;
	auto Render(RenderModule* pRenderModule)->void;
	auto Clone()const->RenderObject*;
protected:
	auto Initialize(RenderModule* pRenderModule, std::wstring const& filePath)->HRESULT;
	auto InitializeFrameMatrix(Frame* pFrame)->void;
	auto UpdateFrameMatrices(Frame* pFrame, DirectX::XMFLOAT4X4 const& mat)->void;
private:
	std::shared_ptr<D3DXFRAME> m_pRootFrame;
	std::unique_ptr<HierarchyLoader> m_pHierarchyLoader;
	std::unique_ptr<AnimationController> m_pAnimCtrler;
	std::vector<CustomMeshContainer* > m_meshContainters;
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