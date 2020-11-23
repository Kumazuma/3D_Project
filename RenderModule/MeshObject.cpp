#include"pch.h"
#include"MeshObject.h"
XMeshObject::XMeshObject():
    m_stride{ 0 },
    m_vertexCount{0},
    m_subsetCount{ 0 },
    m_copied{ false }
{

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
        m_pMaterials[index].pTextureFilename,
        strlen(m_pMaterials[index].pTextureFilename),
        szName,
        256);
    return szName;
}

