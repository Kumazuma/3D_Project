#pragma once
//#include <BulletDynamics/Character/btKinematicCharacterController.h>
//
//class CharacterController : public btCharacterControllerInterface
//{
//protected:
//	btScalar m_halfHeight;

//	btRigidBody* m_rigidBody;

//	btConvexShape* m_convexShape;  //is also in m_ghostObject, but it needs to be convex, so we store it here to avoid upcast

//	btScalar m_maxPenetrationDepth;
//	btScalar m_verticalVelocity;
//	btScalar m_verticalOffset;
//	btScalar m_fallSpeed;
//	btScalar m_jumpSpeed;
//	btScalar m_SetjumpSpeed;
//	btScalar m_maxJumpHeight;
//	btScalar m_maxSlopeRadians;  // Slope angle that is set (used for returning the exact value)
//	btScalar m_maxSlopeCosine;   // Cosine equivalent of m_maxSlopeRadians (calculated once when set, for optimization)
//	btScalar m_gravity;

//	btScalar m_turnAngle;

//	btScalar m_stepHeight;

//	btScalar m_addedMargin;  //@todo: remove this and fix the code

//	///this is the desired walk direction, set by the user
//	btVector3 m_walkDirection;
//	btVector3 m_normalizedDirection;
//	btVector3 m_AngVel;

//	btVector3 m_jumpPosition;

//	//some internal variables
//	btVector3 m_currentPosition;
//	btScalar m_currentStepOffset;
//	btVector3 m_targetPosition;

//	btQuaternion m_currentOrientation;
//	btQuaternion m_targetOrientation;

//	///keep track of the contact manifolds
//	btManifoldArray m_manifoldArray;

//	bool m_touchingContact;
//	btVector3 m_touchingNormal;

//	btScalar m_linearDamping;
//	btScalar m_angularDamping;

//	bool m_wasOnGround;
//	bool m_wasJumping;
//	bool m_useGhostObjectSweepTest;
//	bool m_useWalkDirection;
//	btScalar m_velocityTimeInterval;
//	btVector3 m_up;
//	btVector3 m_jumpAxis;

//	static btVector3* getUpAxisDirections();
//	bool m_interpolateUp;
//	bool full_drop;
//	bool bounce_fix;

//	btVector3 computeReflectionDirection(const btVector3& direction, const btVector3& normal);
//	btVector3 parallelComponent(const btVector3& direction, const btVector3& normal);
//	btVector3 perpindicularComponent(const btVector3& direction, const btVector3& normal);

//	bool recoverFromPenetration(btCollisionWorld* collisionWorld);
//	void stepUp(btCollisionWorld* collisionWorld);
//	void updateTargetPositionBasedOnCollision(const btVector3& hit_normal, btScalar tangentMag = btScalar(0.0), btScalar normalMag = btScalar(1.0));
//	void stepForwardAndStrafe(btCollisionWorld* collisionWorld, const btVector3& walkMove);
//	void stepDown(btCollisionWorld* collisionWorld, btScalar dt);
//
//	virtual bool needsCollision(const btCollisionObject* body0, const btCollisionObject* body1);

//	void setUpVector(const btVector3& up);
//
//	btQuaternion getRotation(btVector3& v0, btVector3& v1) const;
//
//public:
//	BT_DECLARE_ALIGNED_ALLOCATOR();

//	CharacterController(btRigidBody* rigidBody, btConvexShape* convexShape, btScalar stepHeight, const btVector3& up = btVector3(1.0, 0.0, 0.0));
//	~CharacterController();

//	///btActionInterface interface
//	virtual void updateAction(btCollisionWorld* collisionWorld, btScalar deltaTime)
//	{
//		preStep(collisionWorld);
//		playerStep(collisionWorld, deltaTime);
//	}

//	///btActionInterface interface
//	void debugDraw(btIDebugDraw* debugDrawer);

//	void setUp(const btVector3& up);

//	const btVector3& getUp() { return m_up; }

//	/// This should probably be called setPositionIncrementPerSimulatorStep.
//	/// This is neither a direction nor a velocity, but the amount to
//	///	increment the position each simulation iteration, regardless
//	///	of dt.
//	/// This call will reset any velocity set by setVelocityForTimeInterval().
//	virtual void setWalkDirection(const btVector3& walkDirection);

//	/// Caller provides a velocity with which the character should move for
//	///	the given time period.  After the time period, velocity is reset
//	///	to zero.
//	/// This call will reset any walk direction set by setWalkDirection().
//	/// Negative time intervals will result in no motion.
//	virtual void setVelocityForTimeInterval(const btVector3& velocity,
//											btScalar timeInterval);

//	virtual void setAngularVelocity(const btVector3& velocity);
//	virtual const btVector3& getAngularVelocity() const;

//	virtual void setLinearVelocity(const btVector3& velocity);
//	virtual btVector3 getLinearVelocity() const;

//	void setLinearDamping(btScalar d) { m_linearDamping = btClamped(d, (btScalar)btScalar(0.0), (btScalar)btScalar(1.0)); }
//	btScalar getLinearDamping() const { return m_linearDamping; }
//	void setAngularDamping(btScalar d) { m_angularDamping = btClamped(d, (btScalar)btScalar(0.0), (btScalar)btScalar(1.0)); }
//	btScalar getAngularDamping() const { return m_angularDamping; }

//	void reset(btCollisionWorld* collisionWorld);
//	void warp(const btVector3& origin);

//	void preStep(btCollisionWorld* collisionWorld);
//	void playerStep(btCollisionWorld* collisionWorld, btScalar dt);

//	void setStepHeight(btScalar h);
//	btScalar getStepHeight() const { return m_stepHeight; }
//	void setFallSpeed(btScalar fallSpeed);
//	btScalar getFallSpeed() const { return m_fallSpeed; }
//	void setJumpSpeed(btScalar jumpSpeed);
//	btScalar getJumpSpeed() const { return m_jumpSpeed; }
//	void setMaxJumpHeight(btScalar maxJumpHeight);
//	bool canJump() const;

//	void jump(const btVector3& v = btVector3(0, 0, 0));

//	void applyImpulse(const btVector3& v) { jump(v); }

//	void setGravity(const btVector3& gravity);
//	btVector3 getGravity() const;

//	/// The max slope determines the maximum angle that the controller can walk up.
//	/// The slope angle is measured in radians.
//	void setMaxSlope(btScalar slopeRadians);
//	btScalar getMaxSlope() const;

//	void setMaxPenetrationDepth(btScalar d);
//	btScalar getMaxPenetrationDepth() const;

//	btRigidBody* GetRigidBody();
//	void setUseGhostSweepTest(bool useGhostObjectSweepTest)
//	{
//		m_useGhostObjectSweepTest = useGhostObjectSweepTest;
//	}

//	bool onGround() const;
//	void setUpInterpolate(bool value);
//};