#pragma once
#include<game/component.hpp>
#include"typedef.hpp"
#include "HitPoints.hpp"
#include "ManaPoints.hpp"
namespace Kumazuma::Client
{
	enum class CharacterClass:u32;
	class COMCharacterStateInfo : public Game::Component
	{
	public:
		COMCharacterStateInfo(HitPoints const& hitPoints, ManaPoints const& manaPoints);
		auto GetManaPoints()const->ManaPoints const&;
		auto GetHitPoints()const->HitPoints const&;
		auto GetManaPoints()->ManaPoints&;
		auto GetHitPoints()->HitPoints&;

		auto Clone()const->Game::Component* override;
	private:
		HitPoints	hitPoints_;
		ManaPoints	manaPoints_;
		DECLARE_COMPONENT_CLASS(Kumazuma::Client::COMCharacterStateInfo)
	};
	enum class CharacterClass :u32
	{
		UnnamedCreature,
		NamedCreature,

	};
}