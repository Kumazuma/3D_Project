#pragma once
#include<BulletDynamics/Character/btCharacterControllerInterface.h>
class btCollisionShape;
class btRigidBody;
class btCollisionWorld;
class btOverlappingPairCache;
class btCollisionDispatcher;
namespace Kumazuma::Client
{
	class DynamicCharacterController :public btCharacterControllerInterface
	{
	public:
		DynamicCharacterController();
		~DynamicCharacterController();
		auto Setup(btScalar height = 2.f, btScalar width = 0.25f, btScalar stepHeight = 0.25f)->void;
		auto Destroy()->void;
	public:
		virtual auto RegisterPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher)->void;
		auto GetCollisionObject()->btCollisionObject*;
		virtual auto setWalkDirection(const btVector3& walkDirection)->void;
		virtual auto setVelocityForTimeInterval(const btVector3& velocity, btScalar timeInterval)->void;
		virtual auto reset(btCollisionWorld* collisionWorld)->void;
		virtual auto warp(const btVector3& origin)->void;

		virtual auto preStep(btCollisionWorld* collisionWorld)->void;
		virtual auto playerStep(btCollisionWorld* collisionWorld, btScalar dt)->void;
		virtual auto canJump() const->bool;
		virtual auto jump(const btVector3& dir = btVector3(0, 0, 0)) -> void;

		virtual auto onGround() const->bool override;
		virtual auto setUpInterpolate(bool value)->void  override;
	protected:
		btScalar halfHeight_;
		btCollisionShape* shape_;
		btRigidBody* rigidBody_;
		btVector3 raySource_[2];
		btVector3 rayTargets_[2];
		btScalar rayLambda_[2];
		btVector3 rayNormal_[2];

		btScalar turnAngle_;

		btScalar maxLinearVelocity_;
		btScalar walkVelocity_;
		btScalar turnVelocity_;
	};
}