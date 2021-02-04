#include "pch.h"
#include <common/COMPtr.hpp>
#include <d3d9.h>
#include <DirectXMath.h>
#include <RenderModule/RenderModule.h>
#include "OBJObject.hpp"
#include "MaptoolRenderer.h"
#include <RenderModule/WavefrontOBJMesh.h>
#include "msclr\marshal_cppstd.h"
#include "MaptoolRenderer.h"
#include "OBJSubsetCollection.hpp"
using namespace msclr::interop;
namespace MaptoolRenderer
{
    using namespace System::ComponentModel;
    using namespace System;
    using namespace DirectX;
    OBJObject::OBJObject(OBJMesh^ mesh)
    {
        mesh_ = mesh;

        subsets_ = gcnew List< OBJSubset^>{};
        for each (auto name in mesh_->subsetNames_)
        {
            subsets_->Add(gcnew OBJSubset{ this, name });
        }
        
        
    }
    OBJObject::OBJObject(OBJObject^ rhs):
        OBJObject(rhs->mesh_)
    {
    }
    auto OBJObject::PrepereRender(GraphicsDevice^ renderer) -> void 
    {
        for each (auto subset in subsets_)
        {
            switch (subset->RenderKind)
            {
            case OBJSubsetRenderKind::Deferred:
                renderer->AddRenderObject(RenderGroupID::Deferred, subset);
                break;
            case OBJSubsetRenderKind::AlphaTest:
                renderer->AddRenderObject(RenderGroupID::AlphaTest, subset);
                break;
            }
        }
    }

    auto OBJObject::Subsets::get()->OBJSubsetCollection^
    {
        return gcnew OBJSubsetCollection{ this, subsets_ };
    }
    OBJSubset::OBJSubset(OBJObject^ parent, String^ name)
    {
        this->objObject_ = parent;
        this->name_ = name;
        this->renderKind_ = OBJSubsetRenderKind::Deferred;
    }
    auto OBJSubset::Render(GraphicsDevice^ renderer) -> void
    {
        marshal_context ctx;
        std::wstring name{ ctx.marshal_as<std::wstring>(name_) };

        UINT passCount{};
        COMPtr<IDirect3DDevice9> device;
        COMPtr<ID3DXEffect> effect;
        WavefrontOBJMesh* mesh{ static_cast<WavefrontOBJMesh*> (objObject_->mesh_->renderObject_) };
        renderer->RenderModuleHandle->GetDevice(&device);
        renderer->GetEffect("deferred", &effect);
        if (effect != nullptr)
        {
            effect->SetMatrix("g_mWorld", reinterpret_cast<D3DXMATRIX*>(objObject_->transformPtr_));
            effect->Begin(&passCount, 0);
            effect->BeginPass(0);
        }
        else
        {
            device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(objObject_->transformPtr_));
        }
        mesh->DrawSubset(name, renderer->RenderModuleHandle, device.Get(), effect.Get());
        if (effect != nullptr)
        {
            effect->EndPass();
            effect->End();
        }
    }
    auto OBJSubset::RenderKind::get()->OBJSubsetRenderKind
    {
        return this->renderKind_;
    }
    auto OBJSubset::RenderKind::set(OBJSubsetRenderKind value)->void
    {
        this->renderKind_ = value;
    }
    auto OBJSubset::Name::get()->String^
    {
        return this->name_;
    }
}