#pragma once
#include<d3d9.h>
#include"Common.h"
#include"RenderObject.h"
#include"RenderEntity.h"
#include<unordered_map>
#include<vector>
#include<memory>
#include"COMPtr.hpp"
class WowMapMeshSubset;
class WowMapMeshEntity;
class DLL_CLASS WowMapMeshObject: public RenderObject
{
	static constexpr DWORD FVF = FVF_TEX;
	static constexpr size_t VERTEX_SIZE = sizeof(VERTEX<FVF>);
	static constexpr D3DFORMAT INDEX_TYPE = D3DFMT_INDEX32;
	static constexpr size_t INDEX_SIZE = sizeof(Index<INDEX_TYPE>);

	friend class WowMapMeshSubset;
	friend class WowMapMeshEntity;
public:
	using Triangle = WowMapMeshObject::Index<WowMapMeshObject::INDEX_TYPE>;
protected:
	WowMapMeshObject(RenderModule* pRenderModule, std::wstring const& path);
	WowMapMeshObject(WowMapMeshObject const&);
	WowMapMeshObject(WowMapMeshObject&&)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& path, WowMapMeshObject** pOut)->HRESULT;
	auto PrepareRender(IRenderer* pRenderer)->void;;
	auto Clone()const->RenderObject*;
	auto GetVertexCount()const->u32;
	auto GetCenter()const->DirectX::XMFLOAT3A const&;
	auto CanRayPicking()const->bool override;
	auto RayPicking(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDirection, f32* pOut)->bool override;
	auto GetVertices()const->std::shared_ptr<const std::vector<DirectX::XMFLOAT3A> >;
	auto GetSubsets()const->std::unordered_map<std::wstring, std::shared_ptr<WowMapMeshSubset> > const&;
private:
	auto ParseOBJFile(RenderModule* pRenderModule, std::wstring const& path, std::wstring* pOutMeterialFilePath)->void;
private:
	u32 m_vertexCount;
	f32 m_radius;
	DirectX::XMFLOAT3A m_center;
	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffers;
	COMPtr<IDirect3DVertexDeclaration9> m_pVertexDecl;
	std::unordered_map<std::wstring, std::shared_ptr<WowMapMeshSubset> > m_subsets;
	std::unordered_map<std::wstring, COMPtr<IDirect3DTexture9> > m_textures;
	std::shared_ptr < std::vector<DirectX::XMFLOAT3A> > m_pVertexPositions;
	std::vector< std::shared_ptr < WowMapMeshEntity> > m_entities;
};
class DLL_CLASS WowMapMeshSubset
{
public:
	using Triangle = WowMapMeshObject::Index<WowMapMeshObject::INDEX_TYPE>;
	WowMapMeshSubset(RenderModule* renderModule, std::shared_ptr<std::vector<DirectX::XMFLOAT3A> > const & vertexPositions, std::wstring const& materialName, std::vector<Triangle>&& indices);
	WowMapMeshSubset(WowMapMeshSubset&& rhs)noexcept;
	auto GetIndicesRef()const->std::vector<Triangle> const&;
	auto GetIndexBuffer()->COMPtr<IDirect3DIndexBuffer9>;
	auto GetTriangleCount()const->u32;
	auto GetMaterialName()const->std::wstring const&;
	auto GetRadius()const->f32;
	auto GetCenter()const->DirectX::XMFLOAT3A const&;
	//auto RayPicking(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDirection, f32* pOut)->bool ;
	auto __vectorcall RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDirection, f32* pOut)->bool;
	auto GetTriangles()const->std::vector<Triangle> const&;
private:
	DirectX::XMFLOAT3A m_center;
	f32 m_radius;
	std::wstring m_materialName;
	std::vector<Triangle> m_indices;
	std::shared_ptr <std::vector<DirectX::XMFLOAT3A> > m_pVertexPositions;
	COMPtr<IDirect3DIndexBuffer9> m_pIndexBuffer;
};
class WowMapMeshEntity : public RenderEntity
{
public:
	WowMapMeshEntity(WowMapMeshObject* obj, std::wstring const& subsetName, std::shared_ptr<WowMapMeshSubset> const& subset);
	auto Render(RenderModule* pRenderModule, IRenderer* pRenderer)->void override;
	auto GetSubset()->std::shared_ptr<WowMapMeshSubset> const&;
private:
	std::wstring m_subsetName;
	WowMapMeshObject* m_obj;
	std::shared_ptr<WowMapMeshSubset> m_subset;
};