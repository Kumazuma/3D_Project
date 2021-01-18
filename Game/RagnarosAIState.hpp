#pragma once
#include<string>
#include"typedef.hpp"
#include<memory>
class RenderObject;
namespace Kumazuma::Game
{
	class Object;
}
namespace Kumazuma::Client
{
	class COMRagnarosAI;
	
	class RagnarosAIState
	{
	public:
		static u64 constexpr STATE_WAIT_PLAYER = 0;
		static u64 constexpr STATE_BIRTH = 1;
		static u64 constexpr STATE_PHASE1_CHASING = 2;
		static u64 constexpr STATE_PHASE1_STANCE = 3;
		static u64 constexpr STATE_PHASE1_WAIT = 4;
		static u64 constexpr STATE_PHASE1_ATTACK = 5;
		static u64 constexpr STATE_PHASE2_CHASING = 6;
		static u64 constexpr STATE_PHASE2_ATTACK = 7;
		static f32 const RAGNAROS_WALK_SPEED;
		static f32 const RAGNAROS_ATTACK_DAMEGE;
		static f32 const RAGNAROS_ANGLE_X;
		virtual ~RagnarosAIState() = default;
		auto Update(COMRagnarosAI* com, Game::Object* object, f32 timeDelta)->void;
		auto Reset(COMRagnarosAI* com, Game::Object* object)->void;
	protected:
		auto GetState()const->u64;
		auto SetState(u64 id)->void;
		auto GetObjectRef()->Game::Object&;
		auto GetComponentRef()->COMRagnarosAI&;
		auto CalculateDistanceToTarget(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& target)->f32;
		auto CalculateXYAngleToTarget(DirectX::XMFLOAT3 const& position, DirectX::XMFLOAT3 const& target, DirectX::XMFLOAT2* out)->void;
		auto SetARMColliderTransform(DirectX::XMFLOAT4X4 const& transform)->void;
	protected:
		virtual auto Update(f32 timeDelta)->void = 0;
		virtual auto Reset()->void = 0;
	private:
		std::weak_ptr<RenderObject> armMesh_;
		COMRagnarosAI* com_;
		Game::Object* object_;
	};
}