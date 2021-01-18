#pragma once
#include "typedef.hpp"
#include <memory>
#include <SpinLock.hpp>
namespace Kumazuma::Client
{
	class Preferences
	{
	public:
		Preferences();
		Preferences(Preferences&&) noexcept;
		Preferences(Preferences const&) = default;
		auto operator =(Preferences const&)->Preferences& = default;
		auto operator =(Preferences &&)noexcept->Preferences &;

		static auto Current()->Preferences const&;
		static auto Update(Preferences const&)->void;
		static auto Update(Preferences &&)->void;

		bool showCollisionBox;
	protected:

	private:
		static std::unique_ptr<Preferences> s_instance;
		static SpinLock s_locker;
	};
}
