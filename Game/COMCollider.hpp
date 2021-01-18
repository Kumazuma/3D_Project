#pragma once
#include<game/component.hpp>
#include<string>
#include<memory>
#include"Collder.hpp"
#include"typedef.hpp"
#include<unordered_map>
#include<optional>
#include<shared_mutex>
namespace Kumazuma::Client
{
	class COMCollider : public Game::Component
	{
	public:
		COMCollider();
		COMCollider(COMCollider const& rhs);
		~COMCollider();
		auto Clone()const->Game::Component*;
		auto Add(std::wstring const& name, Collider const& collider)->void;
		auto Remove(std::wstring const& name)->void;
		auto Get(std::wstring const& name)->std::optional<Collider>;
		auto GetColliderTableRef()const->std::unordered_map<std::wstring, Collider> const&;
	private:
		mutable std::shared_mutex lock_;
		std::unordered_map<std::wstring, Collider> colliders_;

		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMCollider)

	};
}