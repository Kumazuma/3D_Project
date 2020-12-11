#include "pch.h"
#include "BoxMeshObj.h"
#include <RenderModule/SimpleColliderBoxObject.h>
#include"SkinnedXMeshObj.h"
using namespace System::ComponentModel;
#pragma unmanaged
inline void CreateWorldTransform(
    DirectX::XMFLOAT4X4* matrix,
    DirectX::XMFLOAT3 const& position,
    DirectX::XMFLOAT3 const& rotation,
    DirectX::XMFLOAT3 const& scale)
{
    using namespace DirectX;
    XMVECTOR vRotation{ XMLoadFloat3(&rotation) };
    vRotation = vRotation * 180.f / XM_PI;

    XMMATRIX mScale{ XMMatrixScaling(scale.x, scale.y,scale.z) };
    XMMATRIX mRotation{ XMMatrixRotationRollPitchYawFromVector(vRotation) };
    XMMATRIX mWorld{ mScale * mRotation };
    mWorld.r[3] = XMVectorSetW(XMLoadFloat3(&position), 1.f);
    XMStoreFloat4x4(matrix, mWorld);
}
inline void CombineMatrix(
    DirectX::XMFLOAT4X4* matrix1,
    DirectX::XMFLOAT4X4* matrix2,
    DirectX::XMFLOAT4X4* out
)
{
    using namespace DirectX;
    XMStoreFloat4x4(out, XMLoadFloat4x4(matrix1) * XMLoadFloat4x4(matrix2));
}
#pragma managed

MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(GraphicsDevice^ device)
{
    HRESULT hr{};
    SimpleBoxObject* pBox{};
    hr = SimpleBoxObject::Create(device->Handle, 1.f, 1.f, 1.f, &pBox);
    if (FAILED(hr))
    {
        throw gcnew System::Exception(L"error!!!!");
    }
    m_pNativeObject = pBox;
    pBox->SetColor(1.f, 0.f, 0.f, 1.f);
    m_pParentMatrix = new DirectX::XMFLOAT4X4{ };
    (*m_pParentMatrix)(0, 0) = 1;
    (*m_pParentMatrix)(1, 1) = 1;
    (*m_pParentMatrix)(2, 2) = 1;
    (*m_pParentMatrix)(3, 3) = 1;

}

MapToolRender::BoxColliderMeshObject::BoxColliderMeshObject(BoxColliderMeshObject^ const& rhs) :
    ColliderRenderObject(rhs)
{
    m_pParentMatrix = new DirectX::XMFLOAT4X4{ *rhs->m_pParentMatrix };
}

MapToolRender::BoxColliderMeshObject::~BoxColliderMeshObject()
{
    if (m_pParentMatrix != nullptr)
    {
        delete m_pParentMatrix;
        m_pParentMatrix = nullptr;
    }
}

MapToolRender::BoxColliderMeshObject::!BoxColliderMeshObject()
{
    this->~BoxColliderMeshObject();
}

auto MapToolRender::BoxColliderMeshObject::Clone() -> RenderObject^ 
{
    return gcnew BoxColliderMeshObject(this);
}

auto MapToolRender::BoxColliderMeshObject::SetFrameMatrix(SkinnedXMeshObj^ skinnedMesh, String^ frameName) -> void 
{
    using namespace DirectX;
    auto pFrame{ skinnedMesh->Frames[frameName] };
    if (pFrame == nullptr)
    {
        throw gcnew System::NullReferenceException("Frame is Null, Can not Find Frame");
    }
    *m_pParentMatrix = *pFrame;
    UpdateTransform();
}

auto MapToolRender::BoxColliderMeshObject::SetAttribute(MapToolCore::ColliderAttribute^ attribute) -> void 
{
    auto sphareAttr{ dynamic_cast<MapToolCore::BoxColliderAttribute^>(attribute) };
    if (sphareAttr == nullptr)
    {
        throw gcnew System::Exception("can not apply this attribute");
    }
    Width = sphareAttr->Width;
    Height = sphareAttr->Height;
    Depth = sphareAttr->Depth;
}

auto MapToolRender::BoxColliderMeshObject::UpdateTransform() -> void 
{
    using namespace DirectX;
    XMFLOAT4X4 worldMatrix;
    XMFLOAT3 position{ m_transform->Position.X, m_transform->Position.Y,m_transform->Position.Z };
    XMFLOAT3 scale{ m_transform->Scale.X, m_transform->Scale.Y,m_transform->Scale.Z };
    XMFLOAT3 rotation{ m_transform->Rotation.X, m_transform->Rotation.Y,m_transform->Rotation.Z };
    CreateWorldTransform(&worldMatrix, position, rotation, scale);
    CombineMatrix(&worldMatrix, m_pParentMatrix, &worldMatrix);
    m_pNativeObject->SetTransform(worldMatrix);
}
auto MapToolRender::BoxColliderMeshObject::Depth::get() -> float
{
    return m_depth;
}
auto MapToolRender::BoxColliderMeshObject::Width::get() -> float
{
    return m_width;
}
auto MapToolRender::BoxColliderMeshObject::Height::get() -> float
{
    return m_height;
}
auto MapToolRender::BoxColliderMeshObject::Depth::set(float value) ->void
{
    m_depth = value;
    static_cast<SimpleBoxObject*>(m_pNativeObject)->SetSize(m_width, m_height, m_depth);
}
auto MapToolRender::BoxColliderMeshObject::Width::set(float value) ->void
{
    m_width = value;
    static_cast<SimpleBoxObject*>(m_pNativeObject)->SetSize(m_width, m_height, m_depth);

}
auto MapToolRender::BoxColliderMeshObject::Height::set(float value) ->void
{
    m_height= value;
    static_cast<SimpleBoxObject*>(m_pNativeObject)->SetSize(m_width, m_height, m_depth);

}
auto MapToolRender::BoxColliderMeshObject::Offset::get() -> MapToolCore::Offset
{
    return m_offset;
}

auto MapToolRender::BoxColliderMeshObject::Offset::set(MapToolCore::Offset value) -> void
{
    m_offset = value;
    static_cast<SimpleBoxObject*>(m_pNativeObject)->SetOffset(m_offset.X, m_offset.Y, m_offset.Z);
}
