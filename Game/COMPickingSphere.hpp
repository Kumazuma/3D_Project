#pragma once
#include<game/component.hpp>
#include"typedef.hpp"
#include<optional>
namespace Kumazuma::Client
{
	class COMPickingSphere :public Game::Component
	{
	public:
		COMPickingSphere(f32x3 const& offset, f32 radius);
		auto Clone()const->Game::Component*;
		auto GetRadius()const->f32;
		auto GetOffset()const->f32x3 const&;
		auto SetRadius(f32 radius)->void;
		auto SetOffset(f32x3 const& offset)->void;
		auto PickingTest(f32x3 const& rayAt, f32x3 const& dir)const->std::optional<f32>;
	private:
		f32x3	offset_;
		f32		radius_;
		
		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMPickingSphere)
	};
}