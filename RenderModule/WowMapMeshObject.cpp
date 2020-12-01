#include "pch.h"
#include "WowMapMeshObject.h"
#include "UnicodeHelper.h"
#include <DirectXMath.h>
#include <fstream>
#include <array>
#undef max
#undef min
using namespace DirectX;
WowMapMeshObject::WowMapMeshObject(RenderModule* pRenderModule, std::wstring const& path):
    RenderObject{}
{
    std::wstring materialFileName{};
    ParseOBJFile(pRenderModule, path, &materialFileName);
    size_t slash1{ path.find_last_of(L'\\') };
    size_t slash2{ path.find_last_of(L'/') };
    size_t slash{ std::wstring::npos };

    if (slash1 != std::wstring::npos && slash2 == std::wstring::npos)
        slash = slash1;
    else if (slash1 == std::wstring::npos && slash2 != std::wstring::npos)
        slash = slash2;
    else if (slash1 != std::wstring::npos && slash2 != std::wstring::npos)
        slash = std::max(slash1, slash2);
    if(slash != std::wstring::npos)
        materialFileName = path.substr(0, slash + 1) + materialFileName;
    ParseMtlFile(pRenderModule, materialFileName);
    for (auto& it : m_subsets)
    {
        m_entities.emplace_back(std::make_shared<WowMapMeshEntity>(this, it.first, it.second));
    }
    XMVECTOR vMax{ XMVectorSet(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),0.f) };
    XMVECTOR vMin{ XMVectorSet(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),0.f) };
    for (auto& positions:*m_pVertexPositions)
    {
        XMVECTOR vPos;
        for (u32 i = 0; i < 3; ++i)
        {
            vPos = XMLoadFloat3A(&positions);
            vMax = XMVectorMax(vPos, vMax);
            vMin = XMVectorMin(vPos, vMin);
        }
    }
    XMVECTOR vCenter{ (vMin + vMax) * 0.5f };
    XMStoreFloat(&m_radius, XMVector3Length(vCenter - vMin));
    XMStoreFloat3A(&m_center, vCenter);

}
auto WowMapMeshObject::ParseOBJFile(RenderModule* pRenderModule, std::wstring const& path, std::wstring* pOutMeterialFilePath) -> void
{
    HRESULT hr{};
    std::vector<XMFLOAT3A> positions;
    std::vector<XMFLOAT2A> UVs;
    std::vector<XMFLOAT3A> normals;
    std::unordered_map<std::wstring, std::vector<std::array<XMINT3, 3 > > > groups;
    std::unordered_map<std::wstring, std::wstring > materialNames;

    std::wstring currentGroup;
    std::wstring currentMaterial;
    std::wstring materialLibName;
    std::ifstream fileStream;
    std::unordered_map<std::wstring, std::vector<std::array<XMINT3, 3 > > >::iterator it{ groups.end() };
    fileStream.open(path);
    if (!fileStream.is_open())
    {
        throw E_FAIL;
    }
    std::string token;
    while (!fileStream.eof())
    {
        fileStream >> token;
        if (token.empty())
        {
            continue;
        }
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
            fileStream.ignore(1024, '\n');
        }
        else if (token == OBJ_TOKEN_GROUP)
        {
            //새 그룹이 생기면 인덱스 버퍼를 바꾼다.
            std::string groupName;
            fileStream >> groupName;
            currentGroup = ConvertUTF8ToWide(groupName);
            it = groups.emplace(currentGroup, std::vector<std::array<XMINT3, 3> >{}).first;
        }
        else if (token == OBJ_TOKEN_FACE)
        {
            std::array<XMINT3, 3> triangle{};
            for (auto& v : triangle)
            {
                fileStream >> v.x;
                fileStream.ignore(1);
                fileStream >> v.y;
                fileStream.ignore(1);
                fileStream >> v.z;
            }
            it->second.emplace_back(triangle);
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
        else
        {
            fileStream.ignore(1024, '\n');
        }
    }
    //Z축을 뒤집는다. 최적화를 믿자. SIMD가 안 적용될 수가 없다.
    for (auto& it : positions)
    {
        it.z *= -1.f;
    }
    //UV의 V를 뒤집어 줘야 한다.
    for (auto& it : UVs)
    {
        it.y = 1.f - it.y;
    }
    //normal의 Z값을 뒤집어 줘야 한다.
    for (auto& it : normals)
    {
        it.z *= -1.f;
    }
    //버텍스 버퍼를 구축하자.
    std::vector<VERTEX<FVF>> vertices;
    vertices.reserve(positions.size());
    const u32 minCount{static_cast<u32>( std::min(UVs.size(), std::min(positions.size(), normals.size())) )};
    m_pVertexPositions = std::make_unique<std::vector<XMFLOAT3A> >();
    for (u32 i = 0; i < minCount; ++i)
    {
        VERTEX<FVF> newVertex{};
        newVertex.vNormal = normals[i];
        newVertex.vUV = UVs[i];
        newVertex.vPosition = positions[i];
        vertices.emplace_back(newVertex);
        m_pVertexPositions->emplace_back(positions[i]);
    }
    for (auto& it : groups)
    {
        std::vector<Index<INDEX_TYPE> > indices;
        indices.reserve(it.second.size());
        for (auto& triangle : it.second)
        {
            Index<INDEX_TYPE> newIndex{};
            for (u32 i = 0; i < 3; ++i)
            {
                if (triangle[i].x == triangle[i].y && triangle[i].y == triangle[i].z)
                {
                    newIndex[i] = triangle[i].x - 1;
                }
                else
                {
                    auto& pos = positions[triangle[i].x - 1];
                    auto& uv = UVs[triangle[i].y - 1];
                    auto& normal = normals[triangle[i].z - 1];
                    auto findIt = std::find_if(
                        vertices.begin() + minCount,
                        vertices.end(),
                        [pos, uv, normal](VERTEX<FVF> const& item)->bool
                        {
                            return
                                item.vPosition == pos &&
                                item.vUV == uv &&
                                item.vNormal == normal;
                        });
                    if (findIt == vertices.end())
                    {
                        VERTEX<FVF> newVertex{};
                        newVertex.vNormal = normal;
                        newVertex.vUV = uv;
                        newVertex.vPosition = pos;
                        vertices.emplace_back(newVertex);
                        m_pVertexPositions->emplace_back(pos);
                        findIt = vertices.end() - 1;
                    }
                    newIndex[i] = findIt - vertices.begin();
                }
                
            }
            indices.emplace_back(newIndex);
        }
        for (auto& triangle: indices)
        {
            std::swap(triangle[0], triangle[2]);
        }
        WowMapMeshSubset subset{ pRenderModule,*m_pVertexPositions, materialNames[it.first], std::move(indices) };
        m_subsets.emplace(it.first, std::make_shared<WowMapMeshSubset>(std::move(subset)));
    }
    //
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    hr = pDevice->CreateVertexBuffer(
        vertices.size() * VERTEX_SIZE,
        0,
        FVF,
        D3DPOOL_MANAGED,
        &m_pVertexBuffers,
        nullptr
    );
    if (FAILED(hr))
    {
        throw E_FAIL;
    }
    VERTEX<FVF>* pVertices{};
    m_pVertexBuffers->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
    memcpy_s(pVertices, vertices.size() * VERTEX_SIZE, vertices.data(), vertices.size() * VERTEX_SIZE);
    m_pVertexBuffers->Unlock();
    m_vertexCount = vertices.size();

    *pOutMeterialFilePath = materialLibName;
}

auto WowMapMeshObject::ParseMtlFile(RenderModule* pRenderModule, std::wstring const& path) -> void
{
    size_t slash1{ path.find_last_of(L'\\') };
    size_t slash2{ path.find_last_of(L'/') };
    size_t slash{ std::wstring::npos };
    std::wstring dir{};
    if (slash1 != std::wstring::npos && slash2 == std::wstring::npos)
        slash = slash1;
    else if (slash1 == std::wstring::npos && slash2 != std::wstring::npos)
        slash = slash2;
    else if (slash1 != std::wstring::npos && slash2 != std::wstring::npos)
        slash = std::max(slash1, slash2);
    if (slash != std::wstring::npos)
        dir = path.substr(0, slash + 1) ;

    std::ifstream fileStream;
    std::wstring materialName;
    std::unordered_map<std::wstring, COMPtr<IDirect3DTexture9> > textures;

    fileStream.open(path);
    if (!fileStream.is_open())
    {
        throw E_FAIL;
    }
    std::string token;
    while (!fileStream.eof())
    {
        fileStream >> token;
        if (token == "newmtl")
        {
            fileStream.ignore();
            std::string name;
            std::getline(fileStream, name);
            materialName = ConvertUTF8ToWide(name);
        }
        else if (token == "map_Kd")
        {
            std::string t;
            fileStream.ignore();
            std::getline(fileStream, t);
            std::wstring textureName = ConvertUTF8ToWide(t);
            auto it = textures.find(textureName);
            if (it == textures.end())
            {
                HRESULT hr{};
                std::wstring texturePath{ dir + textureName };
                COMPtr<IDirect3DTexture9> texture;
                hr = pRenderModule->CreateTexture(texturePath.c_str(), &texture);
                if (FAILED(hr))
                {
                    throw hr;
                }
                it = textures.emplace(textureName, std::move(texture)).first;
            }
            m_textures.emplace(materialName, it->second);
        }
        else
        {
            fileStream.ignore(1024, '\n');
        }
    }
}

WowMapMeshObject::WowMapMeshObject(WowMapMeshObject const& rhs):
    RenderObject{ rhs },
    m_pVertexBuffers{rhs.m_pVertexBuffers},
    m_subsets{ rhs.m_subsets },
    m_textures{rhs.m_textures},
    m_vertexCount{rhs.m_vertexCount},
    m_pVertexPositions{ rhs.m_pVertexPositions },
    m_radius{ rhs.m_radius },
    m_center{ rhs.m_center }

{
    for (auto& it : m_subsets)
    {
        m_entities.emplace_back(std::make_shared<WowMapMeshEntity>(this, it.first, it.second));
    }
}

WowMapMeshObject::WowMapMeshObject(WowMapMeshObject&& rhs) noexcept :
    RenderObject{ rhs },
    m_pVertexBuffers{std::move(rhs.m_pVertexBuffers)},
    m_subsets{std::move(rhs.m_subsets)},
    m_entities{std::move(rhs.m_entities)},
    m_textures{ std::move(rhs.m_textures) },
    m_vertexCount{ rhs.m_vertexCount },
    m_pVertexPositions{ std::move(rhs.m_pVertexPositions)},
    m_radius{std::move(rhs.m_radius)},
    m_center{std::move(rhs.m_center)}
{
    rhs.m_vertexCount = 0;
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

auto WowMapMeshObject::PrepareRender(RenderModule* pRenderModule) -> void
{
    auto& frustum{ pRenderModule->GetFrustum() };
    XMMATRIX mWorldTransform{ XMLoadFloat4x4(&m_transform) };
    XMVECTOR t = XMVector3TransformCoord(XMLoadFloat3A(&m_center), mWorldTransform);
    XMVECTOR radius = XMVector3TransformNormal(XMVectorSet(m_radius, m_radius, m_radius, 0.f), mWorldTransform);

    if (!frustum.Intersact(t, m_radius))
    {
        return;
    }
    for (auto& it : m_entities)
    {
        XMVECTOR t = XMVector3TransformCoord(XMLoadFloat3A(&it->GetSubset()->GetCenter() ), XMLoadFloat4x4(&m_transform));
        if (frustum.Intersact(t, m_radius))
        {
            pRenderModule->AddRenderEntity(RenderModule::Kind::NONALPHA, it);
        }
    }
}

auto WowMapMeshObject::Clone() const -> RenderObject*
{
    return new WowMapMeshObject{ *this };
}

auto WowMapMeshObject::GetVertexCount() const -> u32
{
    return m_vertexCount;
}
auto WowMapMeshObject::GetCenter() const -> DirectX::XMFLOAT3A const&
{
    return m_center;
}

WowMapMeshSubset::WowMapMeshSubset(RenderModule* pRenderModule, std::vector<DirectX::XMFLOAT3A> const& vertexPositions, std::wstring const& materialName, std::vector<Triangle>&& indices):
    m_materialName{materialName}
{
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    HRESULT hr{};
    hr = pDevice->CreateIndexBuffer(
        WowMapMeshObject::INDEX_SIZE * indices.size(),
        0,
        WowMapMeshObject::INDEX_TYPE,
        D3DPOOL_MANAGED,
        &m_pIndexBuffer,
        nullptr
    );
    if (FAILED(hr))
    {
        throw E_FAIL;
    }
    void* pIndices{};
    m_pIndexBuffer->Lock(0, 0, &pIndices, 0);
    memcpy_s(pIndices, WowMapMeshObject::INDEX_SIZE * indices.size(), indices.data(), WowMapMeshObject::INDEX_SIZE * indices.size());
    m_pIndexBuffer->Unlock();
    
    XMVECTOR vMax{ XMVectorSet(std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),std::numeric_limits<float>::min(),0.f) };
    XMVECTOR vMin{ XMVectorSet(std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),0.f) };
    for (auto& triangle : indices)
    {
        XMVECTOR vPos;
        for (u32 i = 0; i < 3; ++i)
        {
            vPos = XMLoadFloat3A(&vertexPositions[triangle[i]]);
            vMax = XMVectorMax(vPos, vMax);
            vMin = XMVectorMin(vPos, vMin);
        }
    }
    XMVECTOR vCenter{ (vMin + vMax) * 0.5f };
    XMStoreFloat(&m_radius, XMVector3Length(vCenter - vMin));
    XMStoreFloat3A(&m_center, vCenter);

    m_indices.swap(indices);
}

WowMapMeshSubset::WowMapMeshSubset(WowMapMeshSubset&& rhs) noexcept:
    m_indices{std::move( rhs.m_indices)},
    m_materialName{std::move(rhs.m_materialName)},
    m_pIndexBuffer{std::move(rhs.m_pIndexBuffer)},
    m_radius{std::move(rhs.m_radius)},
    m_center{std::move(rhs.m_center)}
{
    
}

auto WowMapMeshSubset::GetIndicesRef() const -> std::vector<Triangle> const&
{
    return m_indices;
}

auto WowMapMeshSubset::GetIndexBuffer() -> COMPtr<IDirect3DIndexBuffer9>
{
    return m_pIndexBuffer;
}

auto WowMapMeshSubset::GetTriangleCount() const -> u32
{
    return static_cast<u32>(m_indices.size());
}

auto WowMapMeshSubset::GetMaterialName() const -> std::wstring const&
{
    return m_materialName;
}

auto WowMapMeshSubset::GetRadius() const -> f32
{
    return m_radius;
}

auto WowMapMeshSubset::GetCenter() const -> DirectX::XMFLOAT3A const&
{
    return m_center;
}


WowMapMeshEntity::WowMapMeshEntity(
    WowMapMeshObject* obj,
    std::wstring const& subsetName,
    std::shared_ptr<WowMapMeshSubset> const& subset):
    m_subsetName{ subsetName },
    m_obj{ obj },
    m_subset{ subset }
{
}

auto WowMapMeshEntity::Render(RenderModule* pRenderModule) -> void
{
    auto& subset{ m_obj->m_subsets[m_subsetName] };
    COMPtr<IDirect3DDevice9> pDevice{};
    COMPtr<IDirect3DVertexBuffer9> pVertexBuffer{ m_obj->m_pVertexBuffers };
    COMPtr<IDirect3DIndexBuffer9> pIndexBuffer{ subset->GetIndexBuffer() };
    u32 triangleCount{ subset->GetTriangleCount() };
    pRenderModule->GetDevice(&pDevice);

    pDevice->SetFVF(WowMapMeshObject::FVF);
    pDevice->SetTransform(D3DTS_WORLD, &reinterpret_cast<D3DMATRIX&>(m_obj->m_transform));
    pDevice->SetStreamSource(0, pVertexBuffer.Get(), 0, WowMapMeshObject::VERTEX_SIZE);
    pDevice->SetIndices(pIndexBuffer.Get());
    const auto textureIt{ m_obj->m_textures.find(subset->GetMaterialName()) };
    COMPtr<IDirect3DTexture9> pTexture;
    if (textureIt == m_obj->m_textures.end())
    {
        pRenderModule->GetDefaultTexture(&pTexture);
    }
    else
    {
        pTexture = textureIt->second;
    }

    pDevice->SetTransform(D3DTS_WORLD, &reinterpret_cast<D3DMATRIX&>(m_obj->m_transform));
    pDevice->SetTexture(0, pTexture.Get());
    pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_obj->GetVertexCount(), 0, triangleCount);
}

auto WowMapMeshEntity::GetSubset() -> std::shared_ptr<WowMapMeshSubset> const&
{
    return m_subset;
}
