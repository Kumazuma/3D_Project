#include "pch.h"
#include <RenderModule/SimpleColliderSphareObject.h>
#include "MaptoolRenderer.h"
#include "SphereMesh.hpp"
#include <msclr\lock.h>
#include "msclr\marshal_cppstd.h"
#include <msclr\marshal.h>
using namespace msclr::interop;
MaptoolRenderer::SphereMeshPtr::SphereMeshPtr()
{
    SimpleSphereObject* obj;
    SimpleSphereObject::Create(GraphicsDevice::Instance->RenderModuleHandle, &obj);
    this->ptr_ = obj;
    count_ = 1;
}

MaptoolRenderer::SphereMeshPtr::!SphereMeshPtr()
{
    this->~SphereMeshPtr();
}

MaptoolRenderer::SphereMeshPtr::~SphereMeshPtr()
{
    if (this->ptr_ != nullptr)
    {
        delete this->ptr_;
        this->ptr_ = nullptr;
    }
}

MaptoolRenderer::SphereMesh::SphereMesh()
{
    SphereMeshPtr^ ptr{};
    if (s_ptr == nullptr)
    {
        msclr::lock r(WeakReference<SphereMeshPtr^>::typeid);
        if (s_ptr == nullptr)
        {
            s_ptr = gcnew WeakReference<SphereMeshPtr^>(nullptr);
        }
    }
    if (!s_ptr->TryGetTarget(ptr))
    {
        msclr::lock r(SphereMeshPtr::typeid);
        if (!s_ptr->TryGetTarget(ptr))
        {
            ptr = gcnew SphereMeshPtr{};
            s_ptr->SetTarget(ptr);
        }
    }
    s_ptr->TryGetTarget(ptr);
    renderObject_ = ptr->ptr_;
}

MaptoolRenderer::SphereMesh::~SphereMesh()
{
    renderObject_ = nullptr;
    msclr::lock r(inner_);
    --inner_->count_;
    if (inner_->count_ == 0)
    {
        inner_->~SphereMeshPtr();
        inner_ = nullptr;
    }
}

MaptoolRenderer::SphereMesh::!SphereMesh()
{
    this->~SphereMesh();
}
