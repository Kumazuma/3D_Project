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

private:
	std::shared_ptr<D3DXFRAME> m_pRootFrame;
	std::unique_ptr<HierarchyLoader> m_pHierarchyLoader;
	std::unique_ptr<AnimationController> m_pAnimCtrler;
	std::vector<std::unique_ptr<CustomMeshContainer> > m_meshContainters;
};
struct SkinnedXMeshObject::Frame : public D3DXFRAME
{
	DirectX::XMFLOAT4X4 combinedTransformationMatrix;
};
struct SkinnedXMeshObject::CustomMeshContainer : public D3DXMESHCONTAINER
{
	std::vector<COMPtr<IDirect3DTexture9>> textures;
	COMPtr<ID3DXMesh> pOriginalMesh;
	u32 boneCount;

	std::vector< DirectX::XMFLOAT4X4> frameOffsetMatries;
	std::vector<std::shared_ptr<DirectX::XMFLOAT4X4> > frameCombinedMatries;
	std::vector< DirectX::XMFLOAT4X4> renderingMatries;
	std::vector<DWORD> adjacency;
};
#include"XMeshHierarchyLoader.h"
