#pragma once
#include"RagnarosAIState.hpp"

namespace Kumazuma::Client
{
	class RagnarosBirthState : public RagnarosAIState
	{
	public:
		virtual auto Update(f32 timeDelta)->void override;
		virtual auto Reset()->void override;
	private:

	};
}