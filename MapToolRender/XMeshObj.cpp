#include "pch.h"
#include "XMeshObj.h"
#include <RenderModule/StaticXMeshObject.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;

MapToolRender::StaticXMeshObj::StaticXMeshObj(GraphicsDevice^ device, System::String^ filePath)
{
    StaticXMeshObject* ref{};
    marshal_context ctx;
    IDirect3DCubeTexture9* pTexture{};

    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(filePath) };

    StaticXMeshObject::Create(device->Handle, szFilePath, &ref);
    this->m_pNativeObject = ref;
    this->m_textures = gcnew TextureCollection{ ref->GetMaterialCount() };
    for (int i = 0; i < this->m_textures->Count; ++i)
    {
        System::String^ textureName = ctx.marshal_as<System::String^>(ref->GetDiffuseTextureName(i));
        System::String^ texturePath =System::IO::Path::Combine( System::IO::Path::GetDirectoryName(filePath) , textureName);
        auto texture = gcnew Texture{ device , texturePath };
        m_textures[i] = texture;
        if (texture != nullptr)
        {
            ref->SetDiffuseTexture(i, texture->Handle);
        }
    }
    this->m_textures->CollectionChanged += gcnew System::Collections::Specialized::NotifyCollectionChangedEventHandler(this, &MapToolRender::StaticXMeshObj::OnCollectionChanged);
}

auto MapToolRender::StaticXMeshObj::Clone() -> RenderObject^
{
    return gcnew StaticXMeshObj{ this };
}

MapToolRender::StaticXMeshObj::StaticXMeshObj(StaticXMeshObj^ const& rhs):
    RenderObject{ rhs },
    m_textures{gcnew TextureCollection{rhs->Textures->Count}}
{
    for (int i = 0; i < rhs->Textures->Count; ++i)
    {
        m_textures[i] = rhs->Textures[i];
    }
    m_textures->CollectionChanged += gcnew NotifyCollectionChangedEventHandler(this, &MapToolRender::StaticXMeshObj::OnCollectionChanged);
}
auto MapToolRender::StaticXMeshObj::Textures::get()->TextureCollection^
{
    return m_textures;
}

void MapToolRender::StaticXMeshObj::OnCollectionChanged(System::Object^ sender, NotifyCollectionChangedEventArgs^ e)
{
    int idx = e->OldStartingIndex;
    static_cast<StaticXMeshObject*>(m_pNativeObject)->SetDiffuseTexture(idx, m_textures[idx]->Handle);
}
