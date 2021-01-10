#include "pxJump.hpp"

Kumazuma::Client::Jump::Jump():
    jump_{ false },
    jumpTime_{0.f},
    v0_{0.f}
{
}

auto Kumazuma::Client::Jump::StartJump(f32 v0) -> void
{
    if (jump_) return;
    jumpTime_ = 0.f;
    v0_ = v0;
    jump_ = true;
}

auto Kumazuma::Client::Jump::StopJump() -> void
{
    if (!jump_) return;
    jump_ = false;
}

auto Kumazuma::Client::Jump::GetAccel(f32 gravity, f32 elapsedTime) -> f32
{
    if (!jump_) return 0.f;
    jumpTime_ += elapsedTime;
    //f(h) = G * t ^ 2 + v0 * t ;
    //d/dt f(h) = 2 G t + v0
    f32 const h = gravity * jumpTime_ * jumpTime_ + v0_ * jumpTime_;
    return h;
}
