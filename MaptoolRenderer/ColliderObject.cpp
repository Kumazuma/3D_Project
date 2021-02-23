#include "pch.h"
#include "COMPtr.hpp"
#include <RenderModule/SimpleColliderSphareObject.h>
#include <RenderModule/SimpleColliderBoxObject.h>
#include <RenderModule/SkinnedXMeshObject.h>
#include <d3dx9.h>
#include "NativeRenderer.hpp"
#include "ColliderObject.hpp"
#include "MaptoolRenderer.h"
#include "Mesh.hpp"
#include "XSkinnedMeshObject.hpp"
#include "SkinnedXMesh.hpp"
#include "BoxMesh.hpp"
#include "SphereMesh.hpp"
#include "msclr\marshal_cppstd.h"
auto constexpr null = nullptr;
using namespace msclr::interop;
namespace MaptoolRenderer
{
    using namespace System::ComponentModel;
    ColliderObject::ColliderObject()
    {
        parent_ = nullptr;
    }

 
    auto ColliderObject::PrepereRender(GraphicsDevice^ renderer) -> void
    {
        renderer->AddRenderObject(RenderGroupID::Wireframe, this);
    }

    auto ColliderObject::Render(GraphicsDevice^ renderer) -> void
    {
        if (mesh_ == nullptr)
        {
            return;
        }
        UINT passCount{};
        COMPtr<IDirect3DDevice9> device;

        COMPtr<ID3DXEffect> effect;
        renderer->RenderModuleHandle->GetDevice(&device);

        D3DXMATRIX mParentMatrix{};
        D3DXMATRIX mFrameMatrix{};
        D3DXMATRIX mTransform{};
        D3DXMATRIX mSize{};
        D3DXMATRIX mOffset{};
        D3DXMatrixIdentity(&mParentMatrix);
        D3DXMatrixIdentity(&mFrameMatrix);

        D3DXMatrixTranslation(&mOffset, offset_.X, offset_.Y, offset_.Z);
        
        if (parent_ != nullptr)
        {
            mParentMatrix = reinterpret_cast<D3DXMATRIX const&>(*parent_->transformPtr_);
            marshal_context ctx;
            if (frameName_ != nullptr)
            {
                std::wstring frameName{ ctx.marshal_as<std::wstring>(frameName_) };
                auto framePtr = static_cast<::SkinnedXMeshObject*>(parent_->mesh_->renderObject_)->FindFrameTransfromByName(frameName);
                if (framePtr != nullptr)
                {
                    mFrameMatrix = reinterpret_cast<D3DXMATRIX const&>(*framePtr);
                }
            }
        }
        if (attribute_ == nullptr)
        {
            D3DXMatrixScaling(&mSize, 1, 1, 1);
        }
        else if (attribute_->GetType() == MapToolCore::BoxColliderAttribute::typeid)
        {
            auto boxAttr{ dynamic_cast<MapToolCore::BoxColliderAttribute^>(attribute_) };
            D3DXMatrixScaling(&mSize, boxAttr->Width, boxAttr->Height, boxAttr->Depth);
        }
        else if (attribute_->GetType() == MapToolCore::SphereColliderAttribute::typeid)
        {
            auto sphereAttr{ dynamic_cast<MapToolCore::SphereColliderAttribute^>(attribute_) };
            D3DXMatrixScaling(&mSize, sphereAttr->Radius, sphereAttr->Radius, sphereAttr->Radius);
        }
        mTransform = mSize * mOffset * mFrameMatrix * mParentMatrix;
        device->SetTransform(D3DTS_WORLD, &mTransform);
        device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        mesh_->renderObject_->Render(renderer->renderer_, nullptr);

        device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

    }
    auto ColliderObject::ParentObject::get()->SkinnedXMeshObject^
    {
        return parent_;
    }
    auto ColliderObject::ParentObject::set(SkinnedXMeshObject^ parent)->void
    {
        parent_ = parent;
        PropertyChanged(this, gcnew PropertyChangedEventArgs("ParentObject"));
    }
    auto ColliderObject::FrameNames::get()->cli::array<String^>^
    {
        if (parent_ != nullptr)
        {
            return parent_->mesh_->FrameNames;
        }
        return nullptr;
    }
    auto ColliderObject::FrameName::get()->String^
    {
        return frameName_;
    }
    auto ColliderObject::FrameName::set(String^ value)->void
    {
        frameName_ = value;
        this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "FrameName" });

    }
    auto ColliderObject::Type::get()->ColliderType
    {
        return type_;
    }
    auto ColliderObject::Type::set(ColliderType value)->void
    {
        if (value == type_)
            return;
        type_ = value;
        switch (type_)
        {
        case ColliderType::None:
            attribute_ = nullptr;
            mesh_ = nullptr;
            break;
        case ColliderType::Box:
            attribute_ = gcnew BoxColliderAttribute();
            mesh_ = gcnew BoxMesh();
            break;
        case ColliderType::Sphere:
            attribute_ = gcnew SphereColliderAttribute();
            mesh_ = gcnew SphereMesh();
            break;
        }
        if (attribute_ != nullptr)
        {
            attribute_->PropertyChanged += gcnew PropertyChangedEventHandler(this, &ColliderObject::OnPropertyChanged);
        }
        this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Type" });
        this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Attribute" });
    }
    auto ColliderObject::Attribute::get()->ColliderAttribute^
    {
        return attribute_;
    }
    auto ColliderObject::Name::get()->String^
    {
        return name_;
    }
    auto ColliderObject::Name::set(String^ value)->void
    {
        name_ = value;
        this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Name" });
    }
    auto ColliderObject::Offset::get()->MapToolCore::Offset
    {
        return offset_;
    }
    auto ColliderObject::Offset::set(MapToolCore::Offset value)->void
    {
        offset_ = value;
        this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Offset" });
    }
}


void MaptoolRenderer::ColliderObject::OnPropertyChanged(System::Object^ sender, System::ComponentModel::PropertyChangedEventArgs^ e)
{
    this->PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs{ "Attribute" });
}
