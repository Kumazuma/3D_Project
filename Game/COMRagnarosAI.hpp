#pragma once
#include<game/component.hpp>
#include"typedef.hpp"
namespace Kumazuma::Game
{
	class UpdateEvent;
}
namespace Kumazuma::Client
{
	class CharacterMeta;
	class RagnarosAIState;
	class DamageEvent;
	class COMRagnarosAI : public Game::Component
	{
	public:
		
		COMRagnarosAI();
	public:
		auto Clone()const->Game::Component*;
		auto Update(Game::UpdateEvent const& event)->void;
		auto SetState(u64 state)->void;
		auto GetState()const->u64;
		auto GetCharacterMetaRef()const->CharacterMeta const&;
		//auto OnDamaged()
		auto OnDamaged(DamageEvent const& evt)->void;
	private:
		std::shared_ptr<const CharacterMeta> characterMeta_;
		std::unordered_map<u64, std::unique_ptr<RagnarosAIState> > aiStates_;
		u64 currentStateID_;
		u64 nextStateID_;
		RagnarosAIState* nextState_;
		RagnarosAIState* currentState_;

		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMRagnarosAI)
	};
}