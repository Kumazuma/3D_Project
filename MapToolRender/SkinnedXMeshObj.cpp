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
    m_filePath{rhs->m_filePath}
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