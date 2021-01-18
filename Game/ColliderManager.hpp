#pragma once
#include<memory>
#include<optional>
#include"typedef.hpp"
#include<unordered_set>
#include<SpinLock.hpp>
namespace Kumazuma::Client
{
	class COMCollider;
	class ColliderManager : public std::enable_shared_from_this<ColliderManager>
	{
	public:
		virtual ~ColliderManager() = default;
		static auto Initialize()->bool;
		static auto Release()->void;
		static auto Instance()->std::shared_ptr<ColliderManager>;
		auto Register(COMCollider* component)->void;
		auto Unregister(COMCollider* component)->void;
		auto Update(f32 timeDelta)->void;
	private:
		std::unordered_set<COMCollider*> colliders_;
		static SpinLock s_spinlock;
		static std::shared_ptr<ColliderManager> s_instance;
	};
}