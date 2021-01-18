#include "PhysXPhysicsManager.hpp"
#include "PhysicsCharacterController.hpp"
#include <WavefrontOBJMesh.h>
#include <Game/runtime.hpp>
#include <mutex>
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include <sstream>
#include <PxPhysics.h>
#include <characterkinematic/PxControllerManager.h>
#include <characterkinematic/PxCapsuleController.h>
#include <extensions/PxDefaultAllocator.h>
#include "pxPhysicsCharacterControllerInner.hpp"
#include <thread>
using namespace DirectX;
using namespace physx;
constexpr f32 CONTACT_OFFSET = 0.01f;
constexpr f32 STEP_OFFSET = 0.05f;
constexpr f32 SLOPE_LIMIT = 0.0f;
constexpr f32 INVISIBLE_WALLS_HEIGHT = 0.0f;
constexpr f32 MAX_JUMP_HEIGHT = 0.0f;
#pragma comment(lib, "PhysX_64.lib")
#pragma comment(lib, "PhysXCommon_64.lib")
#pragma comment(lib, "PhysXCooking_64.lib")
#pragma comment(lib, "PhysXExtensions_static_64.lib")
#pragma comment(lib, "PhysXFoundation_64.lib")
#pragma comment(lib, "PhysXPvdSDK_static_64.lib")
#pragma comment(lib, "PhysXCharacterKinematic_static_64.lib")

void defaultCCTInteraction(const PxControllerShapeHit& hit);
//	#define CONTACT_OFFSET			0.1f
//	#define STEP_OFFSET				0.01f
//	#define STEP_OFFSET				0.1f
//	#define STEP_OFFSET				0.2f
//	#define SLOPE_LIMIT				0.8f
//	#define INVISIBLE_WALLS_HEIGHT	6.0f
//	#define MAX_JUMP_HEIGHT			4.0f
struct ControlledActorDesc
{
	f32 mSlopeLimit = 0.f;
	f32 mContactOffset = 0.f;
	f32 mStepOffset = 0.f;
	f32 mInvisibleWallHeight = 0.f;
	f32 mMaxJumpHeight = 0.f;
	f32 mRadius = 0.f;
	f32 mHeight = 0.f;
	f32 mCrouchHeight = 0.f;
	f32 mProxyDensity = 10.f;
	f32 mProxyScale = 0.9f;
	f32 mVolumeGrowth = 1.5f;
	physx::PxUserControllerHitReport* mReportCallback = nullptr;
	physx::PxControllerBehaviorCallback* mBehaviorCallback = nullptr;
};
namespace Kumazuma::Client
{
	PhysXPhysicsManager::PhysXPhysicsManager()
	{
		using namespace physx;
		char const* const strTransport = "127.0.0.1";
		foundation = PxCreateFoundation(
			PX_PHYSICS_VERSION,
			allocator,
			errorCallback);
		pvd = physx::PxCreatePvd(*foundation);
		physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate(strTransport, 5425, 10);
		pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation, physx::PxTolerancesScale{}, true, pvd);
		cooking = PxCreateCooking(PX_PHYSICS_VERSION, *foundation, PxCookingParams(PxTolerancesScale{}));
		dispatcher = physx::PxDefaultCpuDispatcherCreate(std::thread::hardware_concurrency());

		physx::PxSceneDesc sceneDesc{ physics->getTolerancesScale() };
		sceneDesc.gravity = physx::PxVec3{ 0.f, -10.f, 0.f };
		sceneDesc.cpuDispatcher = dispatcher;
		sceneDesc.simulationEventCallback = this;
		sceneDesc.filterShader = &PhysXPhysicsManager::FilterShader;
		scene = physics->createScene(sceneDesc);

		auto pvdClient{ scene->getScenePvdClient() };
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
		pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
		materials.emplace(L"default", physics->createMaterial(0.5f, 0.5f, 0.1f));
		controllerManager = PxCreateControllerManager(*scene);
	}
	PhysXPhysicsManager::~PhysXPhysicsManager()
	{
		controllerManager->release();
		scene->release();
		dispatcher->release();
		physics->release();
		auto transport{ pvd->getTransport() };
		pvd->release();
		transport->release();
		cooking->release();
		foundation->release();
	}

	auto PhysXPhysicsManager::SetCharacterColliderBox(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth) -> void
	{
	}

	auto PhysXPhysicsManager::SetCharacterColliderSphere(StringLiteral<wchar_t> const& colliderKey, f32 radius) -> void
	{
	}

	auto PhysXPhysicsManager::SetCharacterColliderCylinder(StringLiteral<wchar_t> const& colliderKey, f32 width, f32 height, f32 depth) -> void
	{
	}

	auto PhysXPhysicsManager::SetCharacterColliderCapsule(StringLiteral<wchar_t> const& colliderKey, f32 radius, f32 height) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto it = shapes.find(colliderKey);
		if (it != shapes.end())
		{
			return;

		}
		auto material = materials[L"default"];
		PxShape* capsuleObj = physics->createShape(PxCapsuleGeometry(radius, height * 0.5f), *material);
		shapes.emplace(colliderKey, capsuleObj);
	}

	auto PhysXPhysicsManager::SetMap(std::vector<std::unique_ptr<WavefrontOBJMesh>> const& meshs, std::vector< SimpleTransform> const& transforms) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		PxSceneWriteLock scopedLock(*this->scene);

		for (auto* it : mapTriangleMeshs)
		{
			it->release();
		}
		mapTriangleMeshs.clear();

		std::vector< std::vector<XMFLOAT3A> > verticesList;
		std::vector< std::vector<XMUINT3> > trianglesList;
		verticesList.reserve(meshs.size());
		for (auto const& mesh : meshs)
		{
			std::vector<DirectX::XMUINT3> triangles;
			size_t triangleCount{};
			for (auto const& it : mesh->GetSubsets())
			{
				triangleCount += it.second->GetTriangleCount();
			}
			triangles.reserve(triangleCount);
			for (auto const& it : mesh->GetSubsets())
			{
				auto const& subset{ it.second };
				for (auto const& triangle : subset->GetTriangles())
				{
					triangles.emplace_back(DirectX::XMUINT3{ triangle[0], triangle[1], triangle[2] });
				}
			}
			auto vertices{ mesh->GetVertices() };
			std::vector<XMFLOAT3A> newVertices;
			auto mWorldTransform{ XMLoadFloat4x4(&mesh->GetTransform()) };
			newVertices.assign(vertices->size(), {});
			XMVector3TransformCoordStream(
				newVertices.data(),
				sizeof(XMFLOAT3A),
				vertices->data(),
				sizeof(XMFLOAT3A),
				vertices->size(),
				mWorldTransform);

			PxTriangleMeshDesc meshDesc{};
			
			meshDesc.points.count = newVertices.size();
			meshDesc.points.data = newVertices.data();
			meshDesc.points.stride = sizeof(XMFLOAT3A);
			meshDesc.triangles.count = triangleCount;
			meshDesc.triangles.data = triangles.data();
			meshDesc.triangles.stride = sizeof(XMUINT3);
			PxDefaultMemoryOutputStream writeBuffer;
			PxTriangleMeshCookingResult::Enum result{};
			if (!cooking->validateTriangleMesh(meshDesc))
			{
				//__debugbreak();
			}
			if (!cooking->cookTriangleMesh(meshDesc, writeBuffer, &result))
			{
				__debugbreak();
			}
			switch (result)
			{
			case PxTriangleMeshCookingResult::eFAILURE:
				OutputDebugStringA("PxTriangleMeshCookingResult::eFAILURE");
				__debugbreak();
				break;
			case PxTriangleMeshCookingResult::eLARGE_TRIANGLE:
				OutputDebugStringA("PxTriangleMeshCookingResult::eLARGE_TRIANGLE");
				__debugbreak();
				break;
			case PxTriangleMeshCookingResult::eSUCCESS:
				break;
			}
			PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
			auto triangleMesh = physics->createTriangleMesh(readBuffer);
			PxRigidStatic* staticRigidyBody = physics->createRigidStatic(PxTransform(0.f, 0.f, 0.f));
			

			PxTriangleMeshGeometry triGeom;
			triGeom.triangleMesh = triangleMesh;
			triGeom.scale = PxMeshScale{ 1.f };
			triGeom.meshFlags = PxMeshGeometryFlags{ PxMeshGeometryFlag::eDOUBLE_SIDED};
			//PxMaterial material;
			auto material = materials[L"default"];
			//PxShape* shape(...) = PxGetPhysics().createShape(...);	// reference count is 1
			//actor->attachShape(shape);								// increments reference count
			//shape->release();										// releases user reference, leaving reference count at 1
			PxShape* mashShape = PxRigidActorExt::createExclusiveShape(*staticRigidyBody, triGeom, *material);
			if (mashShape == nullptr)
			{
				__debugbreak();
			}
			scene->addActor(*staticRigidyBody);
			mapMeshRigids.emplace_back(staticRigidyBody);
		}
	}

	auto PhysXPhysicsManager::Update(f32 timeDelta) -> void
	{
		{
			std::lock_guard<SpinLock> guard{ spinLocker_ };
			for (auto* it : this->charContollers_)
			{
				it->PreUpdate(timeDelta);
			}
		
			
			this->scene->simulate(timeDelta);
			this->scene->fetchResults(true);
		}
		
		for (auto* it : this->charContollers_)
		{
			it->PostUpdate();
		}
	}

	auto PhysXPhysicsManager::CreatePhysicsComponent(StringLiteral<wchar_t> colliderKey, f32 mass, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset) -> std::optional<PhysicsComponent>
	{
		assert(false);
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		PxSceneWriteLock scopedLock(*this->scene);

		auto const& collisionShapes{ shapes };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{

			auto shape{ it->second };
			PhysicsComponent res;

			return res;
		}
		return std::nullopt;
	}

	auto PhysXPhysicsManager::CreateCharacterController(f32 radius, f32 height, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset) -> std::optional<PhysicsCharacterController>
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		PxSceneWriteLock scopedLock(*this->scene);

		using namespace physx;
		ControlledActorDesc desc;
		auto material = materials[L"default"];

		desc.mSlopeLimit = SLOPE_LIMIT;
		desc.mContactOffset = CONTACT_OFFSET;
		desc.mStepOffset = STEP_OFFSET;
		desc.mInvisibleWallHeight = INVISIBLE_WALLS_HEIGHT;
		desc.mMaxJumpHeight = MAX_JUMP_HEIGHT;
		desc.mRadius = radius;
		desc.mHeight = height;
		desc.mCrouchHeight = height;

		PxCapsuleControllerDesc capsuleDesc;
		capsuleDesc.height = height;
		capsuleDesc.radius = radius;
		capsuleDesc.climbingMode = PxCapsuleClimbingMode::eCONSTRAINED;
		capsuleDesc.position = { initialPosition.x,initialPosition.y ,initialPosition.z };
		capsuleDesc.density = desc.mProxyDensity;
		capsuleDesc.scaleCoeff = desc.mProxyScale;
		capsuleDesc.material = material;
		capsuleDesc.slopeLimit = desc.mSlopeLimit;
		capsuleDesc.contactOffset = desc.mContactOffset;
		capsuleDesc.stepOffset = desc.mStepOffset;
		capsuleDesc.invisibleWallHeight = desc.mInvisibleWallHeight;
		capsuleDesc.maxJumpHeight = desc.mMaxJumpHeight;
		
		physx::PxUserControllerHitReport* mReportCallback = nullptr;
		physx::PxControllerBehaviorCallback* mBehaviorCallback = nullptr;
		capsuleDesc.reportCallback = desc.mReportCallback;
		capsuleDesc.behaviorCallback = desc.mBehaviorCallback;
		capsuleDesc.volumeGrowth = desc.mVolumeGrowth;
		pxPhysicsCharacterControllerInner* ctrlInner = new pxPhysicsCharacterControllerInner{std::static_pointer_cast<PhysXPhysicsManager>(this->shared_from_this())};
		this->charContollers_.emplace_back(ctrlInner);
		capsuleDesc.reportCallback = this;
		capsuleDesc.behaviorCallback = ctrlInner;
		PxController* ctrl = static_cast<PxBoxController*>(controllerManager->createController(capsuleDesc));
		//ctrl->getActor()->setMinCCDAdvanceCoefficient(0.015f);
		ctrl->setContactOffset(-0.5f);
		ctrlInner->SetCharacterController(ctrl);
		PhysicsCharacterController controller{ ctrlInner, offset, radius, height };
		return std::optional<PhysicsCharacterController>(std::move(controller));
	}

	auto PhysXPhysicsManager::Delete(pxPhysicsCharacterControllerInner* inner) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto it = std::find(this->charContollers_.begin(), this->charContollers_.end(), inner);
		if (it != this->charContollers_.end())
		{
			this->charContollers_.erase(it);
		}
	}

	auto PhysXPhysicsManager::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count) -> void { }

	auto PhysXPhysicsManager::onWake(physx::PxActor** actors, physx::PxU32 count) -> void {}

	auto PhysXPhysicsManager::onSleep(physx::PxActor** actors, physx::PxU32 count) -> void {}

	auto PhysXPhysicsManager::onContact(physx::PxContactPairHeader const& pairHeader, physx::PxContactPair const* pairs, physx::PxU32 nbPairs) -> void {}

	auto PhysXPhysicsManager::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count) -> void {}

	auto PhysXPhysicsManager::onAdvance(physx::PxRigidBody const* const* bodyBuffer, physx::PxTransform const* poseBuffer, const physx::PxU32 count) -> void {}

	auto PhysXPhysicsManager::onShapeHit(const physx::PxControllerShapeHit& hit) -> void 
	{
		defaultCCTInteraction(hit);
	}

	auto PhysXPhysicsManager::FilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0, physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1, physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize) -> physx::PxFilterFlags
	{
		return physx::PxFilterFlags();
	}

}
// PT: I'm forced to duplicate this code here for now, since otherwise "eACCELERATION" is banned
PX_INLINE void addForceAtPosInternal(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup)
{
	/*	if(mode == PxForceMode::eACCELERATION || mode == PxForceMode::eVELOCITY_CHANGE)
		{
			Ps::getFoundation().error(PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__,
				"PxRigidBodyExt::addForce methods do not support eACCELERATION or eVELOCITY_CHANGE modes");
			return;
		}*/

	const PxTransform globalPose = body.getGlobalPose();
	const PxVec3 centerOfMass = globalPose.transform(body.getCMassLocalPose().p);

	const PxVec3 torque = (pos - centerOfMass).cross(force);
	body.addForce(force, mode, wakeup);
	body.addTorque(torque, mode, wakeup);
}


static void addForceAtLocalPos(PxRigidBody& body, const PxVec3& force, const PxVec3& pos, PxForceMode::Enum mode, bool wakeup = true)
{
	//transform pos to world space
	const PxVec3 globalForcePos = body.getGlobalPose().transform(pos);

	addForceAtPosInternal(body, force, globalForcePos, mode, wakeup);
}

void defaultCCTInteraction(const PxControllerShapeHit& hit)
{
	PxRigidDynamic* actor = hit.shape->getActor()->is<PxRigidDynamic>();
	if (actor)
	{
		if (actor->getRigidBodyFlags() & PxRigidBodyFlag::eKINEMATIC)
			return;

		if (0)
		{
			const PxVec3 p = actor->getGlobalPose().p + hit.dir * 10.0f;

			PxShape* shape;
			actor->getShapes(&shape, 1);
			PxRaycastHit newHit;
			PxU32 n = PxShapeExt::raycast(*shape, *shape->getActor(), p, -hit.dir, 20.0f, PxHitFlag::ePOSITION, 1, &newHit);
			if (n)
			{
				// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
				// useless stress on the solver. It would be possible to enable/disable vertical pushes on
				// particular objects, if the gameplay requires it.
				const PxVec3 upVector = hit.controller->getUpDirection();
				const PxF32 dp = hit.dir.dot(upVector);
				//		shdfnd::printFormatted("%f\n", fabsf(dp));
				if (fabsf(dp) < 1e-3f)
					//		if(hit.dir.y==0.0f)
				{
					const PxTransform globalPose = actor->getGlobalPose();
					const PxVec3 localPos = globalPose.transformInv(newHit.position);
					::addForceAtLocalPos(*actor, hit.dir * hit.length * 1000.0f, localPos, PxForceMode::eACCELERATION);
				}
			}
		}

		// We only allow horizontal pushes. Vertical pushes when we stand on dynamic objects creates
		// useless stress on the solver. It would be possible to enable/disable vertical pushes on
		// particular objects, if the gameplay requires it.
		const PxVec3 upVector = hit.controller->getUpDirection();
		const PxF32 dp = hit.dir.dot(upVector);
		//		shdfnd::printFormatted("%f\n", fabsf(dp));
		if (fabsf(dp) < 1e-3f)
			//		if(hit.dir.y==0.0f)
		{
			const PxTransform globalPose = actor->getGlobalPose();
			const PxVec3 localPos = globalPose.transformInv(toVec3(hit.worldPos));
			::addForceAtLocalPos(*actor, hit.dir * hit.length * 1000.0f, localPos, PxForceMode::eACCELERATION);
		}
	}
}
