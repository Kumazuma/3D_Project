#pragma once
#include "Effect.hpp"
#include <typedef.hpp>
#include <combaseapi.h>
#include<random>
#include<memory>
#include <SimplePlane.hpp>
class RenderModule;
namespace Kumazuma::Client
{
	class FlameEffectShareState;
	class FlameEffect : public Graphics::Effect
	{
	public:
		FlameEffect(RenderModule* renderModule);
		auto Render(RenderModule& renderModule, Graphics::GameRenderer& render)->void ;
		auto GetCenter(DirectX::XMFLOAT3* out)const->void;
		auto SetSize(f32 scale)->void;
		auto PlayEffect(f32 timeDelta)->void;
		auto SetPlayLength(f32 length)->void;
		auto SetCenter(f32x3 const& pos)->void;
		auto SetSeek(f32 seek)->void;
		auto Reset()->void;
		auto Clone()const->FlameEffect*;
	private:
		static std::weak_ptr<FlameEffectShareState> s_shareState_;
	private:
		std::shared_ptr<Graphics::SimplePlane> simplePlane_;
		std::shared_ptr<FlameEffectShareState> shareState_;
		std::uniform_int_distribution<int>	range_;
		std::mt19937_64						rand_;
		f32x3		center_;
		f32			seek_;
		f32			length_;
		f32			scale_;
	};
}