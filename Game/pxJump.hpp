#pragma once
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class Jump 
	{
	public:
		Jump();
		f32 v0_;
		f32 jumpTime_;
		bool jump_;
		
		auto StartJump(f32 v0)->void;
		auto StopJump()->void;
		auto GetAccel(f32 gravity, f32 elapsedTime)->f32;

	};
}