#pragma once
#include"ColliderAttribute.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class SphareColliderAttribute : public ColliderAttribute
	{
	public:
		auto GetType()const->ColliderType override;
		auto Clone()const->ColliderAttribute* override;
		auto DoLoadFromJson(nlohmann::json const& data)->void override;

	public:
		auto GetRadius()const->f32 const&;
		auto SetRadius(f32 radius)->void;
	private:
		f32 m_radius;
	};
}