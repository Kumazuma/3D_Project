#include "COMPlayerInput.hpp"
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include "COMRenderObjectContainer.hpp"
#include "COMHeightmap.hpp"
#include "COMCollider.hpp"
#include "COMCharacterStateInfo.hpp"
#include "HeightMap.hpp"
#include "framework.h"
#include "app.h"
#include "PhysicsCharacterController.hpp"
#include <sstream>
#include "Preferences.hpp"
#include "ResourceManager.hpp"
#include "PlayerBehavior.hpp"
#include "PlayerNormalBehavior.hpp"
#include "PlayerJumpBehavior.hpp"
#include "PlayerSkillBehavior.hpp"
#include "COMPlayerRender.hpp"
#include "COMSkills.hpp"
constexpr wchar_t CHARACTER_MESH[]{ L"CHARACTER" };
constexpr wchar_t WEAPON_MESH[]{ L"WEAPON" };
namespace Kumazuma::Client
{
	using namespace DirectX;
	IMPLEMENT_COMPONENT_CLASS(Kumazuma::Client::COMPlayerInput)
	f32 const COMPlayerInput::PLAYER_WALK_SPEED { 12.f };
	COMPlayerInput::COMPlayerInput():
		Component{ TAG },
		m_gravity{}
	{
		Bind(Game::EVT_UPDATE, &COMPlayerInput::Update);
		Bind(EVT_Damage, &COMPlayerInput::OnDamage);
		auto& window{ App::Instance()->GetWindow() };
		auto rc{ window.GetRect() };
		POINT cursorPos{ window.GetClientCursorPos() };
		f32x2 const xy{
			static_cast<f32>(cursorPos.x),
			static_cast<f32>(cursorPos.y)
		};
		f32x2 const size{
			static_cast<f32>(rc.right - rc.left),
			static_cast<f32>(rc.bottom - rc.top)
		};
		XMVECTOR vCurosrPos{ XMLoadFloat2(&xy) };
		XMVECTOR vScreenSize{ XMLoadFloat2(&size) };
		XMVECTOR vAxis{ XMVectorSet(1.f, 1.f, 0.f, 0.f) };
		//0 ~ 2.f
		vCurosrPos = vAxis * (vCurosrPos * 2.f) / vScreenSize;
		vCurosrPos -= vAxis;

		XMStoreFloat2(&prevMousePosition_, vCurosrPos);

		auto resourceMgr{ ResourceManager::Instance() };
		this->characterMeta_ = resourceMgr->GetCharacterMeta(L"player");

		behaivors_.emplace(PlayerBehaviorID::Normal, new PlayerNormalBehavior{});
		behaivors_.emplace(PlayerBehaviorID::Jump, new PlayerJumpBehavior{});
		behaivors_.emplace(PlayerBehaviorID::Attack, new PlayerNormalAttackBehavior{});
		behaivors_.emplace(PlayerBehaviorID::Skill1, new PlayerSkill1Behavior{});
		behaivors_.emplace(PlayerBehaviorID::Skill2, new PlayerSkill2Behavior{});

		nextBehaviorID_ = PlayerBehaviorID::Normal;
	}
	auto COMPlayerInput::Clone() const -> Game::Component*
	{
		return nullptr;
	}
	auto COMPlayerInput::Update(Game::UpdateEvent const& event) -> void
	{
		auto inputMgr{ InputManager::Instance() };
		if (inputMgr->IsDown(PLAYER_INPUT::TOGLE_COLLIDER_BOX))
		{
			auto preferences{ Preferences::Current() };
			preferences.showCollisionBox = !preferences.showCollisionBox;
			Preferences::Update(std::move(preferences));
		}

		if (auto obj{ GetObj().lock() }; obj != nullptr)
		{
			if (this->currentBehavior_ != nullptr)
			{
				this->currentBehavior_->Update(obj.get(), this, event.GetDelta());
			}

			if (nextBehaviorID_.has_value())
			{
				currentBehaviorID_ = *nextBehaviorID_;
				nextBehaviorID_ = std::nullopt;
				currentBehavior_ = behaivors_.find(currentBehaviorID_)->second.get();
				currentBehavior_->Reset(obj.get(), this, event.GetDelta());
			}
			auto skillsCom{ obj->GetComponent<COMSkills>() };
			skillsCom->UpdateSkillInfo(event.GetDelta());
		}
	}
	auto COMPlayerInput::OnDamage(DamageEvent const& evt) -> void
	{
		if (auto object = GetObj().lock(); object != nullptr)
		{
			auto const& damage{ evt.GetDamage() };
			auto state{ object->GetComponent<COMCharacterStateInfo>() };
			if (state == nullptr) return;
			auto& hitPoints{ state->GetHitPoints() };
			hitPoints.Damage(damage.amount);
		}
	}
	auto COMPlayerInput::GetState() const -> PlayerBehaviorID
	{
		return currentBehaviorID_;
	}
	auto COMPlayerInput::SetState(PlayerBehaviorID id) -> void
	{
		if (auto it = behaivors_.find(id); it != behaivors_.end())
		{
			nextBehaviorID_ = id;
		}
	}
	auto COMPlayerInput::GetCharacterMeta() const -> std::shared_ptr<CharacterMeta const>
	{
		return characterMeta_;
	}
}
