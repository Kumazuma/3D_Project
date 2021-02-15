#include "pch.h"
#include "Mesh.hpp"

MaptoolRenderer::Mesh::Mesh()
{
    renderObject_ = nullptr;
}

MaptoolRenderer::Mesh::!Mesh()
{
    this->~Mesh();
}

MaptoolRenderer::Mesh::~Mesh()
{
    if (renderObject_ != nullptr)
    {
        delete renderObject_;
        renderObject_ = nullptr;
    }
}

auto MaptoolRenderer::Mesh::Render(IRendererBase* renderer, ID3DXEffect* effect) -> void
{
}
