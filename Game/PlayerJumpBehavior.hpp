#pragma once
#include"PlayerBehavior.hpp"
namespace Kumazuma::Client
{
	enum class JumpAnimID{START, FLYING, END};
	class PlayerJumpBehavior : public PlayerBehavior
	{
	public:

	protected:
		virtual auto Update(f32 timedelta)->void override;
		virtual auto Reset(f32 timedelta)->void override;
	private:
		JumpAnimID jumpAnimID_;
	};
}