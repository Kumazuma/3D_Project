#include "pch.h"
#include <RenderModule\SkyBoxObject.h>
#include "NativeRenderer.hpp"
#include "SkyBox.hpp"
#include "NativeRenderer.hpp"
#include "MaptoolRenderer.h"
#include "CubeTexture.hpp"
MaptoolRenderer::SkyBox::SkyBox()
{
    SkyBoxObject* mesh;
    SkyBoxObject::Create(GraphicsDevice::Instance->RenderModuleHandle, &mesh);
    mesh_ = mesh;
}
MaptoolRenderer::SkyBox::~SkyBox()
{
    if (mesh_ != nullptr)
    {
        delete mesh_;
        mesh_ = nullptr;
    }
}
MaptoolRenderer::SkyBox::!SkyBox()
{
    this->~SkyBox();
}
auto MaptoolRenderer::SkyBox::PrepereRender(GraphicsDevice^ renderer) -> void
{
    renderer->AddRenderObject(RenderGroupID::Environment, this);
}

auto MaptoolRenderer::SkyBox::Render(GraphicsDevice^ renderer) -> void
{
    if (texture_ != nullptr)
    {
        mesh_->SetDiffuseTexture(texture_->Handle);
        mesh_->Render(renderer->renderer_, nullptr);
    }
}
auto MaptoolRenderer::SkyBox::Texture::get() -> CubeTexture^
{
    return texture_;
}
auto MaptoolRenderer::SkyBox::Texture::set(CubeTexture^ texture) -> void
{
    texture_ = texture;
    PropertyChanged(this, gcnew PropertyChangedEventArgs("Texture"));
}
