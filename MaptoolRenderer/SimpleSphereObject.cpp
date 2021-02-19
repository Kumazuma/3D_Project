#include "pch.h"
#include "COMPtr.hpp"
#include <d3dx9.h>
#include <RenderModule\RenderModule.h>
#include "NativeRenderer.hpp"
#include "SimpleSphereObject.hpp"
#include "SphereMesh.hpp"
#include "MaptoolRenderer.h"

MaptoolRenderer::SphereObject::SphereObject()
{
    mesh_ = gcnew SphereMesh();
}

auto MaptoolRenderer::SphereObject::PrepereRender(GraphicsDevice^ renderer) -> void 
{
    renderer->AddRenderObject(RenderGroupID::Wireframe, this);
}

auto MaptoolRenderer::SphereObject::DoRender(GraphicsDevice^ renderer, float radius, MapToolCore::Transform^ transform) -> void
{
    D3DXMATRIX mTransform{};
    D3DXMATRIX mSize{};
    COMPtr<IDirect3DDevice9> device;
    renderer->RenderModuleHandle->GetDevice(&device);
    DirectX::XMFLOAT3 position{ transform->Position.X, transform->Position.Y , transform->Position.Z };
    DirectX::XMFLOAT3 rotation{ transform->Rotation.X, transform->Rotation.Y , transform->Rotation.Z };
    GenerateTransform(&position, &rotation, transform->Scale, (DirectX::XMFLOAT4X4*)&mTransform);
    D3DXMatrixScaling(&mSize, radius, radius, radius);
    mTransform = mSize * mTransform;
    device->SetTransform(D3DTS_WORLD, &mTransform);
    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    mesh_->renderObject_->Render(renderer->renderer_, nullptr);

    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
