#pragma once
#include "RagnarosAIState.hpp"
namespace Kumazuma::Client
{
	class RagnarosWaitPlayerState : public RagnarosAIState
	{
	public:
		auto Update(f32)->void override;
		auto Reset()->void override;
	};
}