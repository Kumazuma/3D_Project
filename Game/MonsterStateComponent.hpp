#pragma once
#include"macro.hpp"
#include<game/component.hpp>
#include<game/runtime.hpp>
#include<game/ComponentFactory.hpp>
namespace Kumazuma
{
	namespace MS2
	{
		struct IMonsterState;
		enum class MonsterStateId
		{
			NONE,
			WAIT,
			ATTACK,
			STURNED,
			DEFAULT_LUH,
			SCREW_LUH,
			BOSS_LUH
		};
		class MonsterStateComponent : public Kumazuma::Game::Component
		{
		public:
			DECLARE_COM_TAG(Kumazuma::MS2::MonsterStateComponent)
			MonsterStateComponent(MonsterStateId firstStateId, std::map<MonsterStateId, std::shared_ptr<IMonsterState> >&& states);
			MonsterStateComponent();
			void Initialize(MonsterStateId firstStateId, std::map<MonsterStateId, std::shared_ptr<IMonsterState> >&& states);
			void Update(const Kumazuma::Game::UpdateEvent& event);
			bool SetState(MonsterStateId stateId);
			Kumazuma::Game::Component* Clone()const override;
		private:
			std::map<MonsterStateId, std::shared_ptr<IMonsterState> > m_states;
			std::shared_ptr<IMonsterState> m_pCurrentState;
			MonsterStateId m_currentStateId;
		};
	}
}