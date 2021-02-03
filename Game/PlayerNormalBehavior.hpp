#pragma once
#include"PlayerBehavior.hpp"
namespace Kumazuma::Client
{
	enum class PlayerNormalState;
	class PlayerNormalBehavior : public PlayerBehavior
	{
	public:
	protected:
		virtual auto Update(f32 timedelta)->void override;
		virtual auto Reset(f32 timedelta)->void override;
	private:
		PlayerNormalState state_;
	};
}