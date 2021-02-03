#pragma once
#include"typedef.hpp"
#include<game/component.hpp>
#include<game/runtime.hpp>
#include"SkinnedXMeshObject.h"
#include"EventTag.hpp"
#include"CharacterMeta.hpp"
#include<unordered_map>
#include<optional>
namespace Kumazuma::Client
{
	enum class PlayerBehaviorID :u32;
	class PlayerBehavior;
	class COMPlayerInput : public Game::Component
	{
	public:
		static f32 const PLAYER_WALK_SPEED;
		COMPlayerInput();
	public:
		auto Clone()const->Game::Component*;
		auto Update(Game::UpdateEvent const& event)->void;
		auto OnDamage(DamageEvent const& evt)->void;
		auto GetState()const->PlayerBehaviorID;
		auto SetState(PlayerBehaviorID id)->void;
		auto GetCharacterMeta()const->std::shared_ptr<CharacterMeta const>;
	private:
		f32x2									prevMousePosition_;
		f32										m_gravity;
		std::shared_ptr<CharacterMeta const>	characterMeta_;
		std::unordered_map<
			PlayerBehaviorID,
			std::unique_ptr<PlayerBehavior>
		>										behaivors_;
		std::optional<PlayerBehaviorID>			nextBehaviorID_;
		PlayerBehaviorID						currentBehaviorID_;
		PlayerBehavior*							currentBehavior_;

		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMPlayerInput)
	};

	enum class PlayerBehaviorID :u32
	{
		Normal,
		Jump,
		Attack,
		Skill1,
		Skill2,
		Spell,
		Stun
	};
}