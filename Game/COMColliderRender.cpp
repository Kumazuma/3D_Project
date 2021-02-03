#include "COMColliderRender.hpp"
#include <GameRenderer.hpp>
#include <SpinLock.hpp>
#include <mutex>
#include <game/object.hpp>
#include "Preferences.hpp"
#include "COMCollider.hpp"
#include "COMPlayerRender.hpp"
#include "typedef.hpp"
#include "BoxColliderAttribute.hpp"
#include "SphareColliderAttribute.hpp"
using namespace DirectX;
SpinLock g_locker;
std::weak_ptr<SimpleBoxObject> Kumazuma::Client::COMColliderRender::s_boxMesh;
std::weak_ptr<SimpleSphereObject> Kumazuma::Client::COMColliderRender::s_sphereMesh;

Kumazuma::Client::COMColliderRender::COMColliderRender(std::shared_ptr<Graphics::GameRenderer> const& renderer, std::shared_ptr<SkinnedXMeshObject> const& skinnedMesh, std::shared_ptr<Game::Object> const& object)
{
    auto boxMesh{ s_boxMesh.lock() };
    auto sphereMesh{ s_sphereMesh.lock() };
    skinnedMesh_ = skinnedMesh;
    if (boxMesh == nullptr)
    {
        std::lock_guard<decltype(g_locker)> guard{ g_locker };
        if (boxMesh == nullptr)
        {
            SimpleBoxObject* boxMeshPtr{};
            SimpleBoxObject::Create(&renderer->GetRenderModuleRef(), 1.f, 1.f, 1.f, &boxMeshPtr);
            boxMesh.reset(boxMeshPtr);
            s_boxMesh = boxMesh;
        }
    }
    if (sphereMesh == nullptr)
    {
        std::lock_guard<decltype(g_locker)> guard{ g_locker };
        if (sphereMesh == nullptr)
        {
            SimpleSphereObject* sphereMeshPtr{};
            SimpleSphereObject::Create(&renderer->GetRenderModuleRef(), &sphereMeshPtr);
            sphereMesh.reset(sphereMeshPtr);
            s_sphereMesh = sphereMesh;
        }
    }
    object_ = object;
    boxMesh_ = std::move(boxMesh);
    sphereMesh_ = std::move(sphereMesh);
    renderer_ = renderer;
    Bind(Game::EVT_UPDATE, &COMColliderRender::Update);
}
auto Kumazuma::Client::COMColliderRender::Update(Game::UpdateEvent const& evt) -> void
{
    auto preferences{ Preferences::Current() };
    if (preferences.showCollisionBox)
    {
        renderer_->AddRender(
            Graphics::RenderGroupID::Wireframe,
            std::static_pointer_cast<RenderComponent>(shared_from_this())
        );
    }
}

auto Kumazuma::Client::COMColliderRender::Render(RenderModule& renderModule, Graphics::GameRenderer& render) -> void
{
    auto parentTransformCom{ std::shared_ptr< Game::TransformComponent>{} };
    auto colliderCom{ std::shared_ptr<COMCollider>() };
    auto object{ object_.lock() };
    auto transformMatrix{ f32x44{} };
    if (object != nullptr)
    {
        colliderCom = object->GetComponent<COMCollider>();
        parentTransformCom = object->GetComponent<Game::TransformComponent>();
        parentTransformCom->GenerateTransform(&transformMatrix);
    }
    else
    {
        return;
    }
    //auto        parentRenderCom{ parentObject->GetComponent<COMSkinnedMeshRender>() };
    //auto        skinnedMesh{ parentRenderCom->GetMesh() };
    auto        parentTransform{ f32x44{} };
    auto        mParentTransform{ XMMATRIX{} };
    parentTransformCom->GenerateTransform(&parentTransform);
    mParentTransform = XMLoadFloat4x4(&parentTransform);

    COMPtr<IDirect3DDevice9> device;
    DWORD fillMode{};
    DWORD cullMode{};
    renderer_->GetDevice(&device);
    device->GetRenderState(D3DRS_FILLMODE, &fillMode);
    device->GetRenderState(D3DRS_CULLMODE, &cullMode);
    device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
    device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    RenderColliders(device.Get(), renderModule, *renderer_, object, transformMatrix);

    device->SetRenderState(D3DRS_FILLMODE, fillMode);
    device->SetRenderState(D3DRS_CULLMODE, cullMode);
}

auto Kumazuma::Client::COMColliderRender::RenderColliders(IDirect3DDevice9* device, RenderModule& renderModule, Graphics::GameRenderer& render, std::shared_ptr<Game::Object> object, f32x44 const& parentMatrix) -> void
{
    auto        colliderCOM{ object->GetComponent<COMCollider>() };
    if (colliderCOM != nullptr)
    {
        auto const& table{ colliderCOM->GetColliderTableRef() };
        auto        mParentTransform = XMLoadFloat4x4(&parentMatrix);
        auto        renderObjectPtr{ raw_ptr<RenderObject>{ } };

        for (auto const& item : table)
        {
            auto& collider{ item.second };
            auto const& offset{ collider.GetOffset() };
            auto        mOffsetMatrix{ XMMatrixTranslationFromVector(XMLoadFloat3(&offset)) };
            auto        framePtr{ skinnedMesh_->FindFrameTransfromByName(collider.GetFrameName()) };
            auto        mFrameTransform{ XMMATRIX{} };
            auto        mSizeTransform{ XMMATRIX{} };
            auto        renderTransform{ f32x44{} };
            if (framePtr == nullptr)
                continue;
            mFrameTransform = XMLoadFloat4x4(framePtr);

            switch (collider.GetType())
            {
            case ColliderType::BOX:
            {
                renderObjectPtr = boxMesh_.get();
                auto const& attr{ static_cast<BoxColliderAttribute const&>(collider.GetAttributeRef()) };
                mSizeTransform = XMMatrixScaling(
                    attr.GetWidth(),
                    attr.GetHeight(),
                    attr.GetDepth());
            }
            break;
            case ColliderType::SPHERE:
                renderObjectPtr = sphereMesh_.get();
                {
                    auto const& attr{ static_cast<SphereColliderAttribute const&>(collider.GetAttributeRef()) };
                    sphereMesh_->SetRadius(attr.GetRadius());
                    mSizeTransform = XMMatrixScaling(
                        attr.GetRadius(),
                        attr.GetRadius(),
                        attr.GetRadius());
                }
                break;
            }

            XMStoreFloat4x4(&renderTransform, mSizeTransform * mOffsetMatrix * mFrameTransform * mParentTransform);


            device->SetTransform(D3DTS_WORLD, reinterpret_cast<D3DMATRIX*>(&renderTransform));
            renderObjectPtr->Render(renderer_.get(), nullptr);
        }
    }

    auto& children{ object->GetChildren() };
    for (auto const& item : children)
    {
        RenderColliders(device, renderModule, render, item.second, parentMatrix);
    }
}

auto Kumazuma::Client::COMColliderRender::Clone() const -> Game::Component*
{
    return nullptr;
}
