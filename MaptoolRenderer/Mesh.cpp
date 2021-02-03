#include "pch.h"
#include "Mesh.hpp"

MaptoolRenderer::Mesh::Mesh()
{
    renderObject_ = nullptr;
}

MaptoolRenderer::Mesh::!Mesh()
{
    this->Dispose();
}

MaptoolRenderer::Mesh::~Mesh()
{
    if (renderObject_ != nullptr)
    {
        delete renderObject_;
    }
}

auto MaptoolRenderer::Mesh::Render(IRendererBase* renderer, ID3DXEffect* effect) -> void
{
    renderObject_->Render()
}
