#pragma once
#include"ColliderAttribute.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class BoxColliderAttribute : public ColliderAttribute
	{
	public:
		auto GetType()const->ColliderType override;
		auto Clone()const->ColliderAttribute* override;
		auto DoLoadFromJson(nlohmann::json const& data)->void override;
		auto GetOBBRadius(DirectX::XMVECTOR axis, f32x44 const& transform)const->f32 override;
		auto GetOBBMaximumRadius(f32x44 const& transform)const->f32 override;

	public:
		auto SetWidth(f32 width)->void;
		auto SetHeight(f32 height)->void;
		auto SetDepth(f32 depth)->void;
		auto GetWidth()const->f32 const&;
		auto GetHeight()const->f32 const&;
		auto GetDepth()const->f32 const&;
	private:
		auto UpdatePositions()->void;
	private:
		f32x3 m_size;
	};
}