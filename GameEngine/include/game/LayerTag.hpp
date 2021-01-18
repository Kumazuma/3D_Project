#pragma once
#include<xhash>
namespace Kumazuma
{
	namespace Game
	{
		class LayerTag
		{
		public:
			LayerTag(char const* const str);
			LayerTag(LayerTag const&) = delete;
			inline auto operator == (LayerTag const& rhs) const noexcept->bool;
			inline auto operator != (LayerTag const& rhs) const noexcept->bool;
			inline auto operator <  (LayerTag const& rhs) const noexcept->bool;
			inline auto operator >  (LayerTag const& rhs) const noexcept->bool;
		private:
			char const* const str_;
		};
		inline LayerTag::LayerTag(char const* const str):
			str_{ str }
		{

		}

		inline auto LayerTag::operator==(LayerTag const& rhs) const noexcept-> bool
		{
			return this == &rhs;
		}

		inline auto LayerTag::operator!=(LayerTag const& rhs) const noexcept-> bool
		{
			return this != &rhs;
		}

		inline auto LayerTag::operator<(LayerTag const& rhs) const noexcept-> bool
		{
			return this < &rhs;
		}

		inline auto LayerTag::operator>(LayerTag const& rhs) const noexcept-> bool
		{
			return this > &rhs;
		}

	}
}
namespace std
{
	template<>
	struct hash<Kumazuma::Game::LayerTag>
	{
	public:
		auto operator ()(Kumazuma::Game::LayerTag const& r) const noexcept->size_t
		{
			hash<Kumazuma::Game::LayerTag const*> hasher;
			return hasher(reinterpret_cast<Kumazuma::Game::LayerTag const*>( &r ) );
		}
	};
}