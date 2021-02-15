#include "pch.h"
#include "COMPtr.hpp"
#include <d3dx9.h>
#include <RenderModule/SimpleColliderBoxObject.h>
#include "SimpleBoxObject.hpp"
#include "BoxMesh.hpp"
#include "MaptoolRenderer.h"
#include "NativeRenderer.hpp"
MaptoolRenderer::BoxMeshObject::BoxMeshObject(BoxMesh^ mesh)
{
	mesh_ = mesh;
}

auto MaptoolRenderer::BoxMeshObject::PrepereRender(GraphicsDevice^ renderer) -> void
{
    renderer->AddRenderObject(RenderGroupID::Wireframe, this);
}

auto MaptoolRenderer::BoxMeshObject::Render(GraphicsDevice^ renderer) -> void
{
    UINT passCount{};
    COMPtr<IDirect3DDevice9> device;
    COMPtr<ID3DXEffect> effect;
    ::SimpleBoxObject* mesh{ static_cast<::SimpleBoxObject*> (mesh_->renderObject_) };
    renderer->RenderModuleHandle->GetDevice(&device);
    device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(transformPtr_));
    mesh->Render(renderer->renderer_, nullptr);
}
