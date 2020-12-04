#include "pch.h"
#include "WowMapMeshObject.h"
#include "UnicodeHelper.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <fstream>
#include <array>
#include "RenderModule.h"
#include "hash_helper_for_f32x3.h"
#include "Common.h"
#include "intersact.h"
#include <sstream>
#undef max
#undef min
//_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef float _ARGUMENT_TYPE_NAME;
//_CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef size_t _RESULT_TYPE_NAME;
//_NODISCARD size_t operator()(const float _Keyval) const noexcept {
//    return _Hash_representation(_Keyval == 0.0F ? 0.0F : _Keyval); // map -0 to 0
//}
namespace std
{
    using Vertex = RenderObject::VERTEX<RenderObject::FVF_TEX>;

    template<>
    struct hash<Vertex>
    {
        size_t operator()(const Vertex& _Keyval)const noexcept {
            return
                std::_Hash_representation<float>(_Keyval.vPosition.x == 0.f ? 0.f : _Keyval.vPosition.x) ^
                std::_Hash_representation<float>(_Keyval.vPosition.y == 0.f ? 0.f : _Keyval.vPosition.y) ^
                std::_Hash_representation<float>(_Keyval.vPosition.x == 0.f ? 0.f : _Keyval.vPosition.y) ^
                std::_Hash_representation<float>(_Keyval.vNormal.x == 0.f ? 0.f : _Keyval.vNormal.x) ^
                std::_Hash_representation<float>(_Keyval.vNormal.y == 0.f ? 0.f : _Keyval.vNormal.y) ^
                std::_Hash_representation<float>(_Keyval.vNormal.x == 0.f ? 0.f : _Keyval.vNormal.y) ^
                std::_Hash_representation<float>(_Keyval.vUV.x == 0.f ? 0.f : _Keyval.vUV.x) ^
                std::_Hash_representation<float>(_Keyval.vUV.y == 0.f ? 0.f : _Keyval.vUV.y)
                ;
        }
    };
    template<>
    struct equal_to<Vertex>
    {
        _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef Vertex _FIRST_ARGUMENT_TYPE_NAME;
        _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef Vertex _SECOND_ARGUMENT_TYPE_NAME;
        _CXX17_DEPRECATE_ADAPTOR_TYPEDEFS typedef bool _RESULT_TYPE_NAME;
        bool operator()(const Vertex& _Left, const Vertex& _Right) const {
            
            return
                ::equal( _Left.vPosition , _Right.vPosition) &&
                ::equal(_Left.vNormal, _Right.vNormal)&&
                ::equal(_Left.vUV, _Right.vUV);
        }
    };
}

template<size_t stride, size_t count>
inline __m256 __vectorcall Load256(void* source)
{
    __m256 res{};
    unsigned char* s{ reinterpret_cast<unsigned char*>(source) };
    for (size_t i = 0; i < count; ++i)
    {
        res.m256_f32[i] = *reinterpret_cast<float*>(s);
        s += stride;
    }
    return res;
}
template<size_t stride, size_t count>
inline void __vectorcall Store256(__m256 source, void* dest)
{
    unsigned char* destination{ reinterpret_cast<unsigned char*>(dest) };
    for (size_t i = 0; i < count; ++i)
    {
        *reinterpret_cast<float*>(destination) = source.m256_f32[i];
        destination += stride;
    }
}

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
    std::unordered_map<std::wstring, std::vector<std::array<XMUINT3, 3 > > > groups;
    std::unordered_map<std::wstring, std::wstring > materialNames;
    std::unordered_map<VERTEX<FVF>, size_t> itemIndexTable;

    std::equal_to<std::Vertex> asd;
    std::equal_to<std::Vertex> asd2{ asd };
    asd = asd2;

    std::wstring currentGroup;
    std::wstring currentMaterial;
    std::wstring materialLibName;
    std::ifstream fileStream;
    std::unordered_map<std::wstring, std::vector<std::array<XMUINT3, 3 > > >::iterator it{ groups.end() };
    fileStream.open(path);
    if (!fileStream.is_open())
    {
        throw E_FAIL;
    }
    std::string token;
    while (!fileStream.eof())
    {
        token.clear();
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
            it = groups.emplace(currentGroup, std::vector<std::array<XMUINT3, 3> >{}).first;
        }
        else if (token == OBJ_TOKEN_FACE)
        {
            std::array<XMUINT3, 3> triangle{};
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
    //Z축을 뒤집는다.
    __m256 _1{ -1.f ,-1.f , -1.f , -1.f , -1.f , -1.f, - 1.f ,-1.f };
    __m256 Zs{};
    assert(positions.size() <= std::numeric_limits<u32>::max());
    size_t length = positions.size() / 8;
    for (size_t i = 0; i < length; ++i)
    {
        Zs = Load256<sizeof(DirectX::XMFLOAT3A), 8>(&positions[i * 8].z);
        Zs = _mm256_mul_ps(Zs, _1);
        Store256<sizeof(DirectX::XMFLOAT3A), 8>(Zs, &positions[i * 8].z);
    }
    length *= 8;
    for (size_t i = 0; i < positions.size() - length; ++i)
    {
        Zs.m256_f32[i] = positions[i + length].z;
    }
    Zs = _mm256_mul_ps(Zs, _1);
    for (size_t i = 0; i < positions.size() - length; ++i)
    {
        positions[i + length].z = Zs.m256_f32[i];
    }

    //normal의 Z값을 뒤집어 줘야 한다.
    length = normals.size() / 8;
    for (size_t i = 0; i < length; ++i)
    {
        Zs = Load256<sizeof(DirectX::XMFLOAT3A), 8>(&normals[i * 8].z);
        Zs = _mm256_mul_ps(Zs, _1);
        Store256<sizeof(DirectX::XMFLOAT3A), 8>(Zs, &normals[i * 8].z);
    }
    length *= 8;
    for (size_t i = 0; i < normals.size() - length; ++i)
    {
        Zs.m256_f32[i] = normals[i + length].z;
    }
    Zs = _mm256_mul_ps(Zs, _1);
    for (size_t i = 0; i < normals.size() - length; ++i)
    {
        normals[i + length].z = Zs.m256_f32[i];
    }

    //-1에서 1로 바꾸고
    //UV의 V를 뒤집어 줘야 한다.
    _1 = _mm256_mul_ps(_1, _1);
    length = UVs.size() / 8;
    for (size_t i = 0; i < length; ++i)
    {
        Zs = Load256<sizeof(DirectX::XMFLOAT2A), 8>(&UVs[i * 8].y);
        Zs = _mm256_sub_ps(_1 , Zs);
        Store256<sizeof(DirectX::XMFLOAT2A), 8>(Zs, &UVs[i * 8].y);
    }
    length *= 8;
    for (size_t i = 0; i < UVs.size() - length; ++i)
    {
        Zs.m256_f32[i] = UVs[i + length].y;
    }
    Zs = _mm256_sub_ps(_1, Zs);
    for (size_t i = 0; i < UVs.size() - length; ++i)
    {
        UVs[i + length].y = Zs.m256_f32[i];
    }
    //버텍스 버퍼를 구축하자.
    std::vector<VERTEX<FVF>> vertices;
    vertices.reserve(positions.size());
    const u32 minCount{static_cast<u32>( std::min(UVs.size(), std::min(positions.size(), normals.size())) )};
    m_pVertexPositions = std::make_unique<std::vector<XMFLOAT3A> >();

    for (auto& it : groups)
    {
        std::vector<Index<INDEX_TYPE> > indices;
        indices.reserve(it.second.size());
        for (auto& triangle : it.second)
        {
            Index<INDEX_TYPE> newIndex{};
            for (u32 i = 0; i < 3; ++i)
            {
                auto& pos = positions[triangle[i].x - 1];
                auto& uv = UVs[triangle[i].y - 1];
                auto& normal = normals[triangle[i].z - 1];
                VERTEX<FVF> newItem{};
                newItem.vNormal = normal;
                newItem.vUV = uv;
                newItem.vPosition = pos;

                auto findIt = itemIndexTable.find(newItem);

                if (findIt == itemIndexTable.end())
                {
                    size_t newVertexIndex{ m_pVertexPositions->size() };
                    vertices.emplace_back(newItem);
                    m_pVertexPositions->emplace_back(pos);
                    findIt = itemIndexTable.emplace(newItem, newVertexIndex).first;
                }
                newIndex[i] = findIt->second;
            }
            indices.emplace_back(newIndex);
        }
        for (auto& triangle: indices)
        {
            std::swap(triangle[0], triangle[2]);
        }
        WowMapMeshSubset subset{ pRenderModule, m_pVertexPositions, materialNames[it.first], std::move(indices) };
        m_subsets.emplace(it.first, std::make_shared<WowMapMeshSubset>(std::move(subset)));
    }
    //
    vertices.shrink_to_fit();
    m_pVertexPositions->shrink_to_fit();
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    hr = pDevice->CreateVertexBuffer(
        vertices.size() * VERTEX_SIZE,
        D3DUSAGE_WRITEONLY,
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
        token.clear();
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
    int s = 0;
    if (!frustum.Intersact(t, m_radius))
    {
        return;
    }
    for (auto& it : m_entities)
    {
        XMVECTOR t = XMVector3TransformCoord(XMLoadFloat3A(&it->GetSubset()->GetCenter() ), XMLoadFloat4x4(&m_transform));
        float radius{ it->GetSubset()->GetRadius() };
        if (frustum.Intersact(t, radius))
        {
            ++s;
            pRenderModule->AddRenderEntity(RenderModule::Kind::NONALPHA, it);
        }
    }
    if (s != 0)
    {
        std::stringstream ss;
        ss << "culling state: " << s << '/' << m_entities.size() << '\n';
        OutputDebugStringA(ss.str().c_str());
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

auto WowMapMeshObject::CanRayPicking() const -> bool 
{
    return true;
}

auto WowMapMeshObject::RayPicking(DirectX::XMFLOAT3 const& rayAt, DirectX::XMFLOAT3 const& rayDirection, f32* pOut) -> bool 
{
    XMVECTOR vRayAt{ XMLoadFloat3(&rayAt) };
    XMVECTOR vRayDir{ XMLoadFloat3(&rayDirection) };
    XMVECTOR vSphare{ XMLoadFloat3(&m_center) };
    //모든 버텍스를 월드 트랜스폼을 적용하느니, 레이를 변경시키자.
    XMMATRIX mWorldInverse{ XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_transform)) };
    vRayAt = XMVector3TransformCoord(vRayAt, mWorldInverse);
    vRayDir = XMVector3TransformNormal(vRayDir, mWorldInverse);
    f32 t{};
    if (!Intersact(vRayAt, vRayDir, vSphare, m_radius, &t))
    {
        return false;
    }
    t = std::numeric_limits<f32>::max();
    f32 t2{ std::numeric_limits<f32>::max() };

    for (auto& it : m_subsets)
    {
        vSphare = XMLoadFloat3(&it.second->GetCenter());
        f32 radius{ it.second->GetRadius() };
        f32 t3{};
        if (Intersact(vRayAt, vRayDir, vSphare, radius, &t3) )
        {
            if (it.second->RayPicking(vRayAt, vRayDir, &t2) && t2 < t)
            {
                t = t2;
            }
        }
    }
    if (t != std::numeric_limits<f32>::max())
    {
        *pOut = t;
        return true;
    }
    return false;
}

WowMapMeshSubset::WowMapMeshSubset(RenderModule* pRenderModule, std::shared_ptr<std::vector<DirectX::XMFLOAT3A> > const & vertexPositions, std::wstring const& materialName, std::vector<Triangle>&& indices):
    m_materialName{materialName},
    m_pVertexPositions{vertexPositions}
{
    COMPtr<IDirect3DDevice9> pDevice;
    pRenderModule->GetDevice(&pDevice);
    auto const& rVertexPositions{ *vertexPositions };
    HRESULT hr{};
    hr = pDevice->CreateIndexBuffer(
        WowMapMeshObject::INDEX_SIZE * indices.size(),
        D3DUSAGE_WRITEONLY,
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
            vPos = XMLoadFloat3A(&rVertexPositions[triangle[i]]);
            vMax = XMVectorMax(vPos, vMax);
            vMin = XMVectorMin(vPos, vMin);
        }
    }
    XMVECTOR vCenter{ (vMin + vMax) * 0.5f };
    XMStoreFloat(&m_radius, XMVector3Length(vCenter - vMin));
    //m_radius *= 0.01f;
    XMStoreFloat3A(&m_center, vCenter);

    m_indices.swap(indices);
    m_indices.shrink_to_fit();
}

WowMapMeshSubset::WowMapMeshSubset(WowMapMeshSubset&& rhs) noexcept:
    m_indices{std::move( rhs.m_indices)},
    m_materialName{std::move(rhs.m_materialName)},
    m_pIndexBuffer{std::move(rhs.m_pIndexBuffer)},
    m_radius{std::move(rhs.m_radius)},
    m_center{std::move(rhs.m_center)},
    m_pVertexPositions{std::move(rhs.m_pVertexPositions) }

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

auto __vectorcall WowMapMeshSubset::RayPicking(DirectX::XMVECTOR rayAt, DirectX::XMVECTOR rayDirection, f32* pOut) -> bool
{
    std::vector<XMFLOAT3A> const& rVertexPositions{ *m_pVertexPositions };
    f32 t{std::numeric_limits<f32>::max()};
    bool res{ false };
    for (auto& triangleIndices : m_indices)
    {
        XMVECTOR V0{ XMLoadFloat3A(&rVertexPositions[triangleIndices[0]]) };
        XMVECTOR V1{ XMLoadFloat3A(&rVertexPositions[triangleIndices[1]]) };
        XMVECTOR V2{ XMLoadFloat3A(&rVertexPositions[triangleIndices[2]]) };
        f32 t2{};
        if (DirectX::TriangleTests::Intersects(rayAt, rayDirection,V0,V1,V2, t2) && t2 < t)
        {
            t = t2;
            res = true;
        }
    }
    if (res)
    {
        *pOut = t;
    }
    return res;
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
