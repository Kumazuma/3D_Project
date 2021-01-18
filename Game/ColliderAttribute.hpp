#pragma once
#include"json.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	enum class ColliderType : int;
	class ColliderAttribute
	{
	public:
		[[nodiscard]]
		static auto FromType(ColliderType type)->ColliderAttribute*;
		[[nodiscard]]
		static auto LoadFromJson(nlohmann::json const& data)->ColliderAttribute*;

	public:
		virtual ~ColliderAttribute() = default;
		virtual auto GetType()const->ColliderType = 0;
		virtual auto Clone()const->ColliderAttribute* = 0;
		virtual auto GetOBBRadius(DirectX::XMVECTOR axis, f32x44 const& transform)const->f32 = 0;
		virtual auto GetOBBMaximumRadius(f32x44 const& transform)const->f32 = 0;

	protected:
		virtual auto DoLoadFromJson(nlohmann::json const& data)->void = 0;
	};
	enum class ColliderType : int
	{
		NONE,
		BOX,
		SPHERE
	};
}