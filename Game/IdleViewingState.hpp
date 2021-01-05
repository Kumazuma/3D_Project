#pragma once
#include "CameraViewingState.hpp"
#include "typedef.hpp"
namespace Kumazuma::Client
{
	class IdleViewingState : public CameraViewingState
	{
	public:
	protected:
		auto OnUpdate(f32 timeDelta)->void override;
		auto Reset(f32 timeDelta)->void override;
	private:
		
	};
}