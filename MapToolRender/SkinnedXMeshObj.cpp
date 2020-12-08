#include "pch.h"
#include "SkinnedXMeshObj.h"
#include <RenderModule/SkinnedXMeshObject.h>
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;


MapToolRender::SkinnedXMeshObj::SkinnedXMeshObj(GraphicsDevice^ device, System::String^ filePath)
{
    marshal_context ctx;
    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(filePath) };

    SkinnedXMeshObject* obj{};
    SkinnedXMeshObject::Create(device->Handle, szFilePath, &obj);
    this->m_pNativeObject = obj;

    m_frameNames = gcnew array<String^>(obj->GetFrameNamesRef().size());
    int i{ 0 };
    for (auto& it : obj->GetFrameNamesRef())
    {
        String^ s{ ctx.marshal_as<String^>(it) };
        m_frameNames[i] = s;
        ++i;
    }
    m_filePath = filePath;
}

auto MapToolRender::SkinnedXMeshObj::Clone() -> RenderObject^ 
{
    return gcnew SkinnedXMeshObj{ this };
}

auto MapToolRender::SkinnedXMeshObj::PlayAnimIdx(unsigned int idx) -> void
{
    auto pObj{ static_cast<SkinnedXMeshObject*>(m_pNativeObject) };
    pObj->SetAnimationSet(idx);
}

auto MapToolRender::SkinnedXMeshObj::Update(int ms) -> void
{
    auto pObj{ static_cast<SkinnedXMeshObject*>(m_pNativeObject) };
    pObj->PlayAnimation(ms / 1000.f);
}

MapToolRender::SkinnedXMeshObj::SkinnedXMeshObj(SkinnedXMeshObj^ const& rhs):
    RenderObject{ rhs },
    m_filePath{rhs->m_filePath},
    m_frameNames(rhs->m_frameNames)
{
    
}
auto MapToolRender::SkinnedXMeshObj::AnimationCount::get()-> System::UInt32
{
    auto pObj{ static_cast<SkinnedXMeshObject*>(m_pNativeObject) };
    return pObj->GetAnimationCount();
}
auto MapToolRender::SkinnedXMeshObj::MeshFilePath::get()-> System::String^
{
    return m_filePath;
}
auto MapToolRender::SkinnedXMeshObj::FrameNames::get()->array<System::String^>^
{
    auto s = gcnew array<String^>(m_frameNames->Length);
    m_frameNames->CopyTo(s, 0);
    return s;
}