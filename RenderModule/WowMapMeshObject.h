#pragma once
#include<d3d9.h>
#include"Common.h"
#include"RenderObject.h"
#include"RenderModule.h"

#include"COMPtr.hpp"
class WowMapMeshSubset;
class WowMapMeshObject: public RenderObject
{
	static constexpr DWORD FVF = FVF_TEX;
	static constexpr size_t VERTEX_SIZE = sizeof(VERTEX<FVF>);
	static constexpr D3DFORMAT INDEX_TYPE = D3DFMT_INDEX32;
	static constexpr size_t INDEX_SIZE = sizeof(Index<INDEX_TYPE>);
	static constexpr char OBJ_TOKEN_OBJ[] = "o";
	static constexpr char OBJ_TOKEN_GROUP[] = "g";
	static constexpr char OBJ_TOKEN_VERTEX[] ="v";
	static constexpr char OBJ_TOKEN_UV[] ="vt";
	static constexpr char OBJ_TOKEN_NORMAL[] = "vn";
	static constexpr char OBJ_TOKEN_FACE[] = "f";
	static constexpr char OBJ_TOKEN_MATERIAL_LIB[] = "mtllib";
	static constexpr char OBJ_TOKEN_MATERIAL_NAME[] = "usemtl";

	friend class WowMapMeshSubset;
protected:
	WowMapMeshObject(RenderModule* pRenderModule, std::wstring const& path);
	WowMapMeshObject(WowMapMeshObject const&);
	WowMapMeshObject(WowMapMeshObject&&)noexcept;
public:
	static auto Create(RenderModule* pRenderModule, std::wstring const& path, WowMapMeshObject** pOut)->HRESULT;
	auto PrepareRender(RenderModule* pDevice)->void;;
	auto Clone()const->RenderObject*;
private:
	COMPtr<IDirect3DVertexBuffer9> m_pVertexBuffers;
	
};
class WowMapMeshSubset
{
private:

};