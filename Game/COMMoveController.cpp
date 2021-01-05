#include "COMMoveController.hpp"
#include "PhysicsManager.hpp"
#include <Game/object.hpp>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/Character/btKinematicCharacterController.h>

#include <sstream>
using namespace DirectX;
#undef min
#undef max
namespace Kumazuma::Client
{
	Game::ComponentTag<COMMoveController> const COMMoveController::TAG{ "COMMoveController" };
	COMMoveController::COMMoveController(f32 speed):
		Component{ TAG },
		speed_{speed}
	{

	}
	auto COMMoveController::Move(f32 timeDelta, DirectX::XMVECTOR dir) -> void
	{
		auto object{ GetObj().lock() };
		if (object == nullptr)
			return;
		auto physicesCom{ object->GetComponent<PhysicsComponent>() };
		if (physicesCom == nullptr)
			return;
		auto colliderObj{ physicesCom->GetCharacterContoller() };
		XMVECTOR implusVector = dir * speed_;
		
		btVector3 implus;
		XMStoreFloat3(reinterpret_cast<XMFLOAT3*>(&implus), implusVector);
		colliderObj->setWalkDirection(implus);
		/*auto linearVelocity{ body->getLinearVelocity() };
		f32 const y{ linearVelocity.y() };
		linearVelocity.setY(0.f);
		if (linearVelocity.length() > speed_)
		{
			linearVelocity = linearVelocity.normalized()* speed_;
		}
		linearVelocity.setY(y);
		body->setLinearVelocity(linearVelocity);*/
	}
	auto COMMoveController::Jump(f32 timeDelta) -> void
	{
		auto object{ GetObj().lock() };
		if (object == nullptr)
			return;
		auto physicesCom{ object->GetComponent<PhysicsComponent>() };
		if (physicesCom == nullptr)
			return;
		auto colliderObj{ physicesCom->GetCharacterContoller() };
		colliderObj->jump({ 0.f, speed_ * 4.f, 0.f });
	}
	auto COMMoveController::Clone() const -> Game::Component*
	{
		return new COMMoveController{ *this };
	}
}
