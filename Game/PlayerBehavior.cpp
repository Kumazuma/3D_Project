#include "PlayerBehavior.hpp"
_BEGIN_NS(Kumazuma::Client)
auto PlayerBehavior::Update(Game::Object* object, COMPlayerInput* component, f32 timedelta) -> void
{
	this->object_ = object;
	this->com_ = component;
	Update(timedelta);
}
auto PlayerBehavior::Reset(Game::Object* object, COMPlayerInput* component, f32 timedelta) -> void
{
	this->object_ = object;
	this->com_ = component;
	Reset(timedelta);
}
auto PlayerBehavior::SetState(PlayerBehaviorID id) -> void
{
	com_->SetState(id);
}
auto PlayerBehavior::GetState() const -> PlayerBehaviorID
{
	return com_->GetState();
}
auto PlayerBehavior::GetObjectRef() -> Game::Object&
{
	return *object_;
}
auto PlayerBehavior::GetComponentRef() -> COMPlayerInput&
{
	return *com_;
}
_END_NS

