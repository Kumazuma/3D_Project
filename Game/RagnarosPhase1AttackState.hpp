#pragma once
#include "RagnarosAIState.hpp"
#include <unordered_set>
namespace Kumazuma::Client
{
	class RagnarosPhase1AttackState : public RagnarosAIState
	{
	public:
		auto Update(f32)->void override;
		auto Reset()->void override;
	private:
		std::unordered_set<Game::Object*> objects_;
	};
}