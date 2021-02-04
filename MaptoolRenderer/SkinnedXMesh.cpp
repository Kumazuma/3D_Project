#include "pch.h"
#include<RenderModule/SkinnedXMeshObject.h>
#include "MaptoolRenderer.h"
#include "SkinnedXMesh.hpp"
#include "msclr\marshal_cppstd.h"
using namespace msclr::interop;
MaptoolRenderer::SkinnedXMesh::SkinnedXMesh(String^ path)
{
    marshal_context ctx;
    std::wstring szFilePath{ ctx.marshal_as<std::wstring>(path) };
    SkinnedXMeshObject* mesh;
    SkinnedXMeshObject::Create(GraphicsDevice::Instance->RenderModuleHandle, szFilePath, &mesh);
    if (mesh == nullptr)
    {

    }
    this->renderObject_ = mesh;
    m_frameNames = gcnew array<String^>(mesh->GetFrameNamesRef().size());
    int i{ 0 };
    for (auto& it : mesh->GetFrameNamesRef())
    {
        String^ s{ ctx.marshal_as<String^>(it) };
        m_frameNames[i] = s;
        ++i;
    }
}

MaptoolRenderer::SkinnedXMesh::SkinnedXMesh(SkinnedXMesh^ rhs)
{
    m_frameNames = rhs->m_frameNames;
    renderObject_ = rhs->renderObject_->Clone();
}

auto MaptoolRenderer::SkinnedXMesh::PlayAnimation(float timeDelta) -> void
{
    static_cast<SkinnedXMeshObject*>(this->renderObject_)->PlayAnimation(timeDelta);
}

auto MaptoolRenderer::SkinnedXMesh::Clone() -> Mesh^
{
    return gcnew SkinnedXMesh(this);
}

auto MaptoolRenderer::SkinnedXMesh::GetFrame(String^ frame, DirectX::XMFLOAT4X4* out) -> void
{
    marshal_context ctx;
    std::wstring frameName{ ctx.marshal_as<std::wstring>(frame) };
    static_cast<SkinnedXMeshObject*>(this->renderObject_)->FindFrameTransfromByName(frameName, out);
}
auto MaptoolRenderer::SkinnedXMesh::AnimationCount::get()-> System::UInt32
{
    auto pObj{ static_cast<SkinnedXMeshObject*>(renderObject_) };
    return pObj->GetAnimationCount();
}
auto MaptoolRenderer::SkinnedXMesh::FrameNames::get()->array<System::String^>^
{
    return m_frameNames;
}