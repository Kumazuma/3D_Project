#include "RagnarosAIState.hpp"
#include "COMRagnarosAI.hpp"
#include "COMRenderObjectContainer.hpp"
#include <RenderObject.h>
#include <game/object.hpp>
#include "Preferences.hpp"
namespace Kumazuma::Client
{
    f32 const RagnarosAIState::RAGNAROS_WALK_SPEED{ 6.f };
    f32 const RagnarosAIState::RAGNAROS_ATTACK_DAMEGE{ 50.f };
    f32 const RagnarosAIState::RAGNAROS_ANGLE_X{ DirectX::XMConvertToRadians(15.f) };
    using namespace DirectX;
    auto RagnarosAIState::Update(COMRagnarosAI* com, Game::Object* object, f32 timeDelta) -> void
    {
        this->com_ = com;
        this->object_ = object;
        Update(timeDelta);

    }

    auto RagnarosAIState::Reset(COMRagnarosAI* com, Game::Object* object) -> void
    {
        this->com_ = com;
        this->object_ = object;
        Reset();
    }

    auto RagnarosAIState::GetState() const -> u64
    {
        return com_->GetState();
    }

    auto RagnarosAIState::SetState(u64 id) -> void
    {
        com_->SetState(id);
    }

    auto RagnarosAIState::GetObjectRef() -> Game::Object&
    {
        return *object_;
    }

    auto RagnarosAIState::GetComponentRef() -> COMRagnarosAI&
    {
        return *com_;
    }

    auto RagnarosAIState::CalculateDistanceToTarget(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& target) -> f32
    {
        auto vCurPos{ LoadF32x3(position) };
        auto vTargetPos{ LoadF32x3(target) };
        auto vDelta{ vTargetPos - vCurPos };
        return StoreF32(XMVector3Length(vDelta));
    }

    auto RagnarosAIState::CalculateXYAngleToTarget(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& target, XMFLOAT2* out) -> void
    {
        auto vCurPos{ LoadF32x3(position) };
        auto vTargetPos{ LoadF32x3(target) };
        auto vDelta{ vTargetPos - vCurPos };
        auto vDir{ XMVector3Normalize(vDelta) };
        auto directToTarget{ StoreF32x3(vDir) };
        auto vDirectionXZ{ XMVector3Normalize(XMVectorSetY(vDelta, 0.f)) };
        f32 angleY{ atan2f(directToTarget.x ,directToTarget.z) };
        f32 angleX{ StoreF32(XMVector3AngleBetweenNormals(vDirectionXZ, vDir)) };

        out->y = angleY;
        out->x = angleX;
    }

    //auto RagnarosAIState::SetARMColliderTransform(DirectX::XMFLOAT4X4 const& transform) -> void
    //{
    //    auto const& preferences{ Preferences::Current() };
    //    auto armMesh{ this->armMesh_.lock() };
    //    if (armMesh == nullptr)
    //    {
    //        auto renderObjectContainter{ object_->GetComponent<COMRenderObjectContainer>() };
    //        armMesh = renderObjectContainter->Get(L"ARM_COLLIDER");
    //        this->armMesh_ = armMesh;
    //        if (armMesh == nullptr)
    //        {
    //            return;
    //        }
    //    }
    //    if (preferences.showCollisionBox)
    //    {
    //        armMesh->SetVisible(true);
    //        //armMesh->SetTransform(transform);
    //        //TODO:
    //    }
    //    else
    //    {
    //        armMesh->SetVisible(false);
    //    }
    //}
}
