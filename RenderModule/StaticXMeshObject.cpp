#include "pch.h"
#include "StaticXMeshObject.h"
#include <Windows.h>
auto StaticXMeshObject::Create(RenderModule* pRenderModule, std::wstring const& filePath) -> HRESULT
{
    //TODO:

    return E_NOTIMPL;
}

auto StaticXMeshObject::Render(RenderModule* pRenderModule) -> void 
{ 
    //TODO:

}

auto StaticXMeshObject::Clone() const -> RenderObject*
{
    //TODO:

    return new StaticXMeshObject{ *this };
}


auto StaticXMeshObject::Initialize(RenderModule* pRenderModule, u32 width, u32 height, f32 interval, f32 terrainMaxHeight, u8 const* pArray) -> HRESULT
{
    //TODO:
    return S_OK;
}

auto XMeshObject::SetDiffuseTexture(u32 index, IDirect3DTexture9* pTexture) -> void
{
    m_textures[index] = MakeCOMPtr(std::move(pTexture));
}

auto XMeshObject::GetDiffuseTextureName(u32 index) -> std::wstring
{
    wchar_t szName[256]{};
    MultiByteToWideChar(
        CP_ACP,
        0,
        m_materials[index].pTextureFilename,
        strlen(m_materials[index].pTextureFilename),
        szName,
        256);
    return szName;
}
