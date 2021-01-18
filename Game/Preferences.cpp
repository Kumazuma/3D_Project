#include "Preferences.hpp"
#include <mutex>
namespace Kumazuma::Client
{
    std::unique_ptr<Preferences> Preferences::s_instance{};
    SpinLock Preferences::s_locker{};
    Preferences::Preferences():
        showCollisionBox{ false }
    {

    }

    Preferences::Preferences(Preferences&& rhs) noexcept :
        showCollisionBox{std::move(rhs.showCollisionBox)}

    {

    }

    auto Preferences::operator=(Preferences&& rhs) noexcept-> Preferences&
    {
        Preferences tmp{ std::move(rhs) };
        this->~Preferences();
        new(this)Preferences{ std::move(tmp) };
        // TODO: 여기에 return 문을 삽입합니다.
        return *this;
    }

    auto Preferences::Current() -> Preferences const&
    {
        std::lock_guard<SpinLock> guard{ s_locker };
        if (s_instance == nullptr)
        {
            s_instance.reset(new Preferences{});
        }
        return *s_instance;
    }

    auto Preferences::Update(Preferences const& rhs) -> void
    {
        Update(Preferences{ rhs });
    }

    auto Preferences::Update(Preferences&& rhs) -> void
    {
        std::lock_guard<SpinLock> guard{ s_locker };
        *s_instance = std::move(rhs);
    }

}
