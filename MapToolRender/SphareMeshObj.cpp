#include "pch.h"
#include "SphareMeshObj.h"
#include "SkinnedXMeshObj.h"
#include <RenderModule/SimpleColliderSphareObject.h>
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


using namespace System::ComponentModel;
MapToolRender::SphareMesh::SphareMesh(GraphicsDevice^ device)
{
    SimpleSphereObject* sphareObj{};
    HRESULT hr{};
    hr = SimpleSphereObject::Create(device->Handle, &sphareObj);
    if (FAILED(hr))
    {
        throw gcnew System::Exception("Could not create Sphare Mesh!");
    }
    m_pNativeObject = sphareObj;
    m_pParentMatrix = new DirectX::XMFLOAT4X4{};
    (*m_pParentMatrix)(0, 0) = 1;
    (*m_pParentMatrix)(1, 1) = 1;
    (*m_pParentMatrix)(2, 2) = 1;
    (*m_pParentMatrix)(3, 3) = 1;

}
MapToolRender::SphareMesh::~SphareMesh()
{
    if (m_pParentMatrix != nullptr)
    {
        delete m_pParentMatrix;
        m_pParentMatrix = nullptr;
    }
}
MapToolRender::SphareMesh::!SphareMesh()
{
    this->~SphareMesh();
}
//TODO:
MapToolRender::SphareMesh::SphareMesh(SphareMesh^ const& rhs):
    ColliderRenderObject(this)
{
    m_pParentMatrix = new DirectX::XMFLOAT4X4{};
    (*m_pParentMatrix)(0, 0) = 1;
    (*m_pParentMatrix)(1, 1) = 1;
    (*m_pParentMatrix)(2, 2) = 1;
    (*m_pParentMatrix)(3, 3) = 1;
}

auto MapToolRender::SphareMesh::Clone() -> RenderObject^ 
{
    return gcnew SphareMesh{ this };
}

auto MapToolRender::SphareMesh::SetFrameMatrix(SkinnedXMeshObj^ skinnedMesh, String^ frameName) -> void
{
    using namespace DirectX;
    *m_pParentMatrix = *skinnedMesh->Frames[frameName];
    UpdateTransform();
}
auto MapToolRender::SphareMesh::SetAttribute(MapToolCore::ColliderAttribute^ attribute) -> void 
{
    auto sphareAttr{ dynamic_cast<MapToolCore::SphareColliderAttribute^>(attribute) };
    if (sphareAttr == nullptr)
    {
        throw gcnew System::Exception("can not apply this attribute");
    }
    Radius = sphareAttr->Radius;
}

auto MapToolRender::SphareMesh::UpdateTransform() -> void
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

auto MapToolRender::SphareMesh::Radius::get() -> float
{
    return m_radius;
}

auto MapToolRender::SphareMesh::Radius::set(float value) -> void
{
    m_radius = value;
    static_cast<SimpleSphereObject*>(m_pNativeObject)->SetRadius(m_radius);
}
auto MapToolRender::SphareMesh::Offset::get() -> MapToolCore::Offset
{
    return m_offset;
}

auto MapToolRender::SphareMesh::Offset::set(MapToolCore::Offset value) -> void
{
    m_offset = value;
    static_cast<SimpleSphereObject*>(m_pNativeObject)->SetOffset(m_offset.X, m_offset.Y, m_offset.Z);
}