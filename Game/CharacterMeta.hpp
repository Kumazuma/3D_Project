#pragma once
#include<unordered_map>
#include<string>
#include"Collder.hpp"
#include"json.hpp"
#include"typedef.hpp"
namespace Kumazuma::Client
{
	class CharacterMeta
	{
	protected:
		CharacterMeta(nlohmann::json const& data);
	public:
		CharacterMeta(CharacterMeta&& rhs) noexcept;
		CharacterMeta(CharacterMeta const& rhs);
		[[nodiscard]]
		static auto LoadFromJson(nlohmann::json const& data)->CharacterMeta*;
	public:
		auto GetMeshPathRef()const->std::wstring const&;
	private:
		std::unordered_map<std::wstring, i32> m_animIdTable;
		std::vector<Collider> m_colliders;
		std::wstring m_meshPath;
	};
}