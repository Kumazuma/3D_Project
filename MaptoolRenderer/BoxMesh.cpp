#include "pch.h"
#include <RenderModule/SimpleColliderBoxObject.h>
#include "MaptoolRenderer.h"
#include "BoxMesh.hpp"
#include <msclr\lock.h>
#include "msclr\marshal_cppstd.h"
#include <msclr\marshal.h>
MaptoolRenderer::BoxMeshPtr::BoxMeshPtr()
{
    SimpleBoxObject* obj;
    SimpleBoxObject::Create(GraphicsDevice::Instance->RenderModuleHandle, 1.f, 1.f, 1.f, &obj);
    this->ptr_ = obj;
    count_ = 1;
}

MaptoolRenderer::BoxMeshPtr::!BoxMeshPtr()
{
    this->~BoxMeshPtr();
}

MaptoolRenderer::BoxMeshPtr::~BoxMeshPtr()
{
    if (this->ptr_ != nullptr)
    {
        delete this->ptr_;
        this->ptr_ = nullptr;
    }
}

MaptoolRenderer::BoxMesh::BoxMesh()
{
    BoxMeshPtr^ ptr{};
    if (s_ptr == nullptr)
    {
        msclr::lock r(WeakReference<BoxMeshPtr^>::typeid);
        if (s_ptr == nullptr)
        {
            s_ptr = gcnew WeakReference<BoxMeshPtr^>(nullptr);
        }
    }
    if (!s_ptr->TryGetTarget(ptr))
    {
        msclr::lock r(BoxMeshPtr::typeid);
        if (!s_ptr->TryGetTarget(ptr))
        {
            ptr = gcnew BoxMeshPtr{};
            s_ptr->SetTarget(ptr);
        }
    }
    s_ptr->TryGetTarget(ptr);
    renderObject_ = ptr->ptr_;
}

MaptoolRenderer::BoxMesh::~BoxMesh()
{
    renderObject_ = nullptr;
    msclr::lock r(inner_);
    --inner_->count_;
    if (inner_->count_ == 0)
    {
        inner_->~BoxMeshPtr();
        inner_ = nullptr;
    }
}

MaptoolRenderer::BoxMesh::!BoxMesh()
{
    this->~BoxMesh();
}
