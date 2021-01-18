#pragma once
#include "RagnarosAIState.hpp"
namespace Kumazuma::Client
{
	class RagnarosPhase1ChasingState : public RagnarosAIState
	{
	public:
		auto Update(f32)->void override;
		auto Reset()->void override;
	};
}