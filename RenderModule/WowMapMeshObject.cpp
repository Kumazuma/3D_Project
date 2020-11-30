#include "pch.h"
#include "WowMapMeshObject.h"
#include "UnicodeHelper.h"
#include <DirectXMath.h>
#include <fstream>
using namespace DirectX;
WowMapMeshObject::WowMapMeshObject(RenderModule* pRenderModule, std::wstring const& path):
    RenderObject{}
{

    std::vector<XMFLOAT3A> positions;
    std::vector<XMFLOAT2A> UVs;
    std::vector<XMFLOAT3A> normals;
    std::unordered_map<std::wstring, std::vector<XMINT3> > groups;
    std::unordered_map<std::wstring, std::wstring > materialNames;

    std::wstring currentGroup;
    std::wstring currentMaterial;
    std::wstring materialLibName;
    std::ifstream fileStream;
    std::unordered_map<std::wstring, std::vector<XMINT3> >::iterator it{ groups.end() };
    fileStream.open(path);
    if (fileStream.is_open())
    {
        throw E_FAIL;
    }
    std::string token;
    while (!fileStream.eof())
    {
        fileStream >> token;
        if (token == OBJ_TOKEN_VERTEX)
        {
            XMFLOAT3A pos;
            fileStream >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (token == OBJ_TOKEN_NORMAL)
        {
            XMFLOAT3A normal;
            fileStream >> normal.x >> normal.y >> normal.z;
            normals.push_back(normal);
        }
        else if (token == OBJ_TOKEN_UV)
        {
            XMFLOAT2A uv;
            fileStream >> uv.x >> uv.y;
            UVs.push_back(uv);
        }
        else if (token == OBJ_TOKEN_OBJ)
        {
            //처리 안 해도 될 듯
        }
        else if (token == OBJ_TOKEN_GROUP)
        {
            //새 그룹이 생기면 인덱스 버퍼를 바꾼다.
            std::string groupName;
            fileStream >> groupName;
            currentGroup = ConvertUTF8ToWide(groupName);
            it = groups.emplace(currentGroup, std::vector<XMINT3>{}).first;
        }
        else if (token == OBJ_TOKEN_FACE)
        {
            XMINT3 face;
            fileStream >> face.x >> face.y >> face.z;
            it->second.push_back(face);
        }
        else if (token == OBJ_TOKEN_MATERIAL_NAME)
        {
            std::string matName;
            fileStream >> matName;
            materialNames[currentGroup] = ConvertUTF8ToWide(matName);
        }
        else if (token == OBJ_TOKEN_MATERIAL_LIB)
        {
            std::string libName;
            fileStream >> libName;
            materialLibName = ConvertUTF8ToWide(libName);
        }
    }
    //Z축을 뒤집는다. 최적화를 믿자. SIMD가 안 적용될 수가 없다.
    for (auto& it : positions)
    {
        it.z *= -1.f;
    }
    //UV의 TODO:
}

WowMapMeshObject::WowMapMeshObject(WowMapMeshObject const& rhs):
    RenderObject{ rhs }
{

}

WowMapMeshObject::WowMapMeshObject(WowMapMeshObject&& rhs) noexcept :
    RenderObject{ rhs },
    m_pVertexBuffers{std::move(rhs.m_pVertexBuffers)}
{

}

auto WowMapMeshObject::Create(RenderModule* pRenderModule, std::wstring const& path, WowMapMeshObject** pOut) -> HRESULT
{
    try
    {
        if (pOut == nullptr)
            throw E_POINTER;
        WowMapMeshObject obj{ pRenderModule, path };
        *pOut = new WowMapMeshObject{ obj };
    }
    catch (HRESULT hr)
    {
        return hr;
    }
    return S_OK;
}

auto WowMapMeshObject::PrepareRender(RenderModule* pDevice) -> void
{
    //TODO:
}

auto WowMapMeshObject::Clone() const -> RenderObject*
{
    return new WowMapMeshObject{ *this };
}
