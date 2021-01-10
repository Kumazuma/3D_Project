#include "DCC.hpp"
#include <BulletCollision/CollisionShapes/btMultiSphereShape.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <BulletCollision/CollisionDispatch/btCollisionWorld.h>
#include <LinearMath/btDefaultMotionState.h>

Kumazuma::Client::DynamicCharacterController::DynamicCharacterController()
{
	rayLambda_[0] = 1.0;
	rayLambda_[1] = 1.0;
	halfHeight_ = 1.0;
	turnAngle_ = 0.0;
	maxLinearVelocity_ = 10.0;
	walkVelocity_ = 8.0; // meters/sec
	turnVelocity_ = 1.0; // radians/sec
	shape_ = nullptr;
	rigidBody_ = nullptr;
}

Kumazuma::Client::DynamicCharacterController::~DynamicCharacterController()
{
}

auto Kumazuma::Client::DynamicCharacterController::Setup(btScalar height, btScalar width, btScalar stepHeight) -> void
{
	btVector3 spherePositions[2];
	btScalar sphereRadii[2];

	sphereRadii[0] = width;
	sphereRadii[1] = width;
	spherePositions[0] = btVector3(0.0, (height / btScalar(2.0) - width), 0.0);
	spherePositions[1] = btVector3(0.0, (-height / btScalar(2.0) + width), 0.0);

	halfHeight_ = height / btScalar(2.0);

	shape_ = new btMultiSphereShape(&spherePositions[0], &sphereRadii[0], 2);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 2.0, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0, myMotionState, shape_);
	rigidBody_ = new btRigidBody(cInfo);
	// kinematic vs. static doesn't work
	//m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	rigidBody_->setSleepingThresholds(0.0, 0.0);
	rigidBody_->setAngularFactor(0.0);
}

auto Kumazuma::Client::DynamicCharacterController::Destroy() -> void
{
	if (shape_)
	{
		delete shape_;
		shape_ = nullptr;
	}

	if (rigidBody_)
	{
		delete rigidBody_;
		rigidBody_ = nullptr;
	}
}

auto Kumazuma::Client::DynamicCharacterController::RegisterPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher) -> void
{
	
}

auto Kumazuma::Client::DynamicCharacterController::GetCollisionObject() -> btCollisionObject*
{
	return rigidBody_;
}

auto Kumazuma::Client::DynamicCharacterController::setWalkDirection(const btVector3& walkDirection) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::reset(btCollisionWorld* collisionWorld) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::warp(const btVector3& origin) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::preStep(btCollisionWorld* collisionWorld) -> void
{
	btTransform xform;
	rigidBody_->getMotionState()->getWorldTransform(xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize();
	forward.normalize();

	raySource_[0] = xform.getOrigin();
	raySource_[1] = xform.getOrigin();

	rayTargets_[0] = raySource_[0] + down * halfHeight_ * btScalar(1.1);
	rayTargets_[1] = raySource_[1] + forward * halfHeight_ * btScalar(1.1);

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		ClosestNotMe(btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace
			);
		}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(rigidBody_);

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		rayCallback.m_closestHitFraction = 1.0;
		collisionWorld->rayTest(raySource_[i], raySource_[i], rayCallback);
		if (rayCallback.hasHit())
		{
			rayLambda_[i] = rayCallback.m_closestHitFraction;
		}
		else {
			rayLambda_[i] = 1.0;
		}
	}
}

auto Kumazuma::Client::DynamicCharacterController::playerStep(btCollisionWorld* collisionWorld, btScalar dt) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::canJump() const -> bool
{
    return false;
}

auto Kumazuma::Client::DynamicCharacterController::jump(const btVector3& dir) -> void
{
}

auto Kumazuma::Client::DynamicCharacterController::onGround() const -> bool 
{
    return false;
}

auto Kumazuma::Client::DynamicCharacterController::setUpInterpolate(bool value) -> void 
{
}
