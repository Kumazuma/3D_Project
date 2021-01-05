#pragma once
#include"CameraViewingState.hpp"
namespace Kumazuma::Client
{
	class LookForwardState : public CameraViewingState
	{
	public:
		LookForwardState();
		auto OnUpdate(f32 timeDelta)->void override;
		auto Reset(f32 timeDelta)->void override;
	private:
		f32 maximumRot_;
	};
}