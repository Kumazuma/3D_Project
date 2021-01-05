#include "PhysicsManager.hpp"
#include "btBulletDynamicsCommon.h"
#include <WowMapMeshObject.h>
#include <Game/runtime.hpp>
#include <mutex>
#include <Game/object.hpp>
#include <Game/TransformComponent.hpp>
#include <sstream>
#include <BulletDynamics/Character/btKinematicCharacterController.h>
#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>
using namespace DirectX;

namespace Kumazuma::Client
{
	Game::EventTag<Game::Event> const EVT_PHYSICS_POST_UPDATE{ "EVT_PHYSICS_POST_UPDATE" };
	struct PhysicsManager::Bullet
	{
		btDiscreteDynamicsWorld* world = nullptr;
		btDefaultCollisionConfiguration* config = nullptr;
		btCollisionDispatcher* dispatcher = nullptr;
		btBroadphaseInterface* overlappingPairCache = nullptr;
		btSequentialImpulseConstraintSolver* solver = nullptr;
		btCollisionShape* mapShape = nullptr;
		btCollisionObject* mapObject = nullptr;
		std::unordered_map<StringLiteral<wchar_t>, btCollisionShape*> collisionShapes;
		std::vector< std::vector<DirectX::XMFLOAT3A> > verticesList_;
		std::vector< std::vector<DirectX::XMUINT3> > trianglesList_;
		btTriangleMesh* mapMesh;
		btAlignedObjectArray<btCollisionObject*> objects;
		~Bullet()
		{
			if (mapObject != nullptr)
			{
				mapObject = nullptr;
			}
			if (mapShape != nullptr)
			{
				delete mapShape;
				mapShape = nullptr;
			}
			if (mapMesh != nullptr)
			{
				delete mapMesh;
				mapMesh;
			}
			for (auto& it : collisionShapes)
			{
				delete it.second;
				it.second = nullptr;
			}

			collisionShapes.clear();
			int i;
			for (i = world->getNumCollisionObjects() - 1; i >= 0; i--)
			{
				btCollisionObject* obj = world->getCollisionObjectArray()[i];
				btRigidBody* body = btRigidBody::upcast(obj);
				if (body && body->getMotionState())
				{
					delete body->getMotionState();
				}
				world->removeCollisionObject(obj);
				delete obj;
			}
			delete world;
			delete config;
			delete dispatcher;
			delete overlappingPairCache;
			delete solver;
		}
	};
	SpinLock PhysicsManager::s_instanceChecker = SpinLock{};
	std::shared_ptr< PhysicsManager> PhysicsManager::s_instance{};
	PhysicsManager::PhysicsManager()
	{
		bullet_ = new Bullet{ };
		bullet_->config = new btDefaultCollisionConfiguration{};
		bullet_->dispatcher = new btCollisionDispatcher{ bullet_->config };
		bullet_->overlappingPairCache = new btDbvtBroadphase{};
		bullet_->solver = new btSequentialImpulseConstraintSolver{};
		bullet_->world = new btDiscreteDynamicsWorld(
			bullet_->dispatcher,
			bullet_->overlappingPairCache,
			bullet_->solver,
			bullet_->config);
	}
	PhysicsManager::~PhysicsManager()
	{
		if (bullet_ != nullptr)
		{
			delete bullet_;
		}
	}
	auto PhysicsManager::Initialize() -> void
	{
		if (s_instance == nullptr)
		{
			std::lock_guard<SpinLock> guard{ s_instanceChecker };
			if (s_instance == nullptr)
			{
				s_instance.reset(new PhysicsManager{});
			}
		}
	}
	auto PhysicsManager::Instance() -> std::shared_ptr<PhysicsManager>
	{
		return s_instance;
	}
	auto PhysicsManager::Release() -> void
	{
		std::lock_guard<SpinLock> guard{ s_instanceChecker };
		s_instance.reset();
	}
	auto PhysicsManager::SetCharacterColliderBox(StringLiteral<wchar_t> colliderKey, f32 width, f32 height, f32 depth) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto& collisionShapes{ this->bullet_->collisionShapes };
		btBoxShape* tmpBoxShape{ new btBoxShape{{width, height, depth} } };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
			it->second = tmpBoxShape;
		}
		else
		{
			collisionShapes.emplace(colliderKey, tmpBoxShape);
		}
	}

	auto PhysicsManager::SetCharacterColliderSphere(StringLiteral<wchar_t> colliderKey, f32 radius) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto& collisionShapes{ this->bullet_->collisionShapes };

		btSphereShape* tmpBoxShape{ new btSphereShape{radius} };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
			it->second = tmpBoxShape;
		}
		else
		{
			collisionShapes.emplace(colliderKey, tmpBoxShape);
		}
	}

	auto PhysicsManager::SetCharacterColliderCylinder(StringLiteral<wchar_t> colliderKey, f32 width, f32 height, f32 depth) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto& collisionShapes{ this->bullet_->collisionShapes };

		btCylinderShape* tmpBoxShape{ new btCylinderShape{{width, height, depth} } };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
			it->second = tmpBoxShape;
		}
		else
		{
			collisionShapes.emplace(colliderKey, tmpBoxShape);
		}
	}

	auto PhysicsManager::SetCharacterColliderCapsule(StringLiteral<wchar_t> colliderKey, f32 radius, f32 height) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto& collisionShapes{ this->bullet_->collisionShapes };
		
		btCapsuleShape* tmpBoxShape{ new btCapsuleShape{radius, height } };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
			it->second = tmpBoxShape;
		}
		else
		{
			collisionShapes.emplace(colliderKey, tmpBoxShape);
		}
	}

	auto PhysicsManager::SetMap(std::vector<std::unique_ptr<WowMapMeshObject>> const& meshs) -> void
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		btTriangleMesh* triangleMesh = new btTriangleMesh{};

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
					triangles.emplace_back(DirectX::XMUINT3{ triangle[0], triangle[2], triangle[1] });
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

			btIndexedMesh indexedMesh;

			indexedMesh.m_indexType = PHY_INTEGER;
			indexedMesh.m_numTriangles = triangleCount;
			indexedMesh.m_numVertices = vertices->size();
			indexedMesh.m_triangleIndexBase = reinterpret_cast<u8 const*>(triangles.data());
			indexedMesh.m_triangleIndexStride = sizeof(XMUINT3);
			indexedMesh.m_vertexBase = reinterpret_cast<u8 const*>(newVertices.data());
			indexedMesh.m_vertexStride = sizeof(XMFLOAT3A);
			indexedMesh.m_vertexType = PHY_FLOAT;
			triangleMesh->addIndexedMesh(indexedMesh);

			verticesList.emplace_back(std::move(newVertices));
			trianglesList.emplace_back(std::move(triangles));
		}
		btBvhTriangleMeshShape* mapShape{ new btBvhTriangleMeshShape(triangleMesh,true) };
		f32 margin{ mapShape->getMargin() };
		std::stringstream sos;
		sos << margin << '\n';
		OutputDebugStringA(sos.str().c_str());
		mapShape->setMargin(0.1f);

		btTransform startTransform;
		startTransform.setIdentity();
		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btVector3 localInertia(0, 0, 0);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f, myMotionState, mapShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);
		body->setFriction(1.0f);
		body->setAngularFactor(0.f);
		//body->forceActivationState(DISABLE_DEACTIVATION);
		body->setCollisionFlags( btCollisionObject::CF_STATIC_OBJECT);
		bullet_->world->addRigidBody(body);
		if (bullet_->mapObject != nullptr)
		{
			bullet_->world->removeCollisionObject(bullet_->mapObject);
			delete bullet_->mapObject;
			bullet_->mapObject = nullptr;
		}
		if (bullet_->mapShape != nullptr)
		{
			delete bullet_->mapShape;
			bullet_->mapShape = nullptr;
		}
		if (bullet_->mapMesh != nullptr)
		{
			delete bullet_->mapMesh;
			bullet_->mapMesh = nullptr;
		}
		bullet_->mapObject = body;
		bullet_->mapShape = mapShape;
		bullet_->mapMesh = triangleMesh;
		bullet_->trianglesList_.swap(trianglesList);
		bullet_->verticesList_.swap(verticesList);
	}
	auto PhysicsManager::CreatePhysicsComponent(StringLiteral<wchar_t> colliderKey, f32 mass, DirectX::XMFLOAT3 const& initialPosition, DirectX::XMFLOAT3 const& offset) -> std::optional<PhysicsComponent>
	{
		std::lock_guard<SpinLock> guard{ spinLocker_ };
		auto const& collisionShapes{ bullet_->collisionShapes };
		if (auto it = collisionShapes.find(colliderKey); it != collisionShapes.end())
		{
			auto shape{ it->second };
			PhysicsComponent res;
			btTransform startTransform;
			startTransform.setIdentity();
			startTransform.setOrigin(btVector3(initialPosition.x + offset.x, initialPosition.y + offset.y, initialPosition.z + offset.z));
			
			auto ghostObject = new btPairCachingGhostObject();
			ghostObject->setWorldTransform(startTransform);
			bullet_->overlappingPairCache->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
			ghostObject->setCollisionShape(shape);
			ghostObject->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
			
			//btPairCachingGhostObject * ghostObject, btConvexShape * convexShape, btScalar stepHeight
			auto charCon = new btKinematicCharacterController(ghostObject,static_cast<btConvexShape*>( shape), 0.05f);
			charCon->setGravity(bullet_->world->getGravity());
			
			bullet_->world->addCollisionObject(ghostObject, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
			bullet_->world->addAction(charCon);
			charCon->setMaxJumpHeight(1.5);
			res.collisionObj_ = charCon;
			res.OnCOMDelete = &PhysicsManager::RemovePhysicsComponent;
			res.physicsManager_ = s_instance;
			res.offset_ = { offset.x, offset.y, offset.z };
			return res;
		}
		return std::nullopt;
	}

	auto PhysicsManager::RemovePhysicsComponent(btKinematicCharacterController* ptr) -> void
	{
		auto ghost{ ptr->getGhostObject() };

		bullet_->world->removeCollisionObject(ghost);
		bullet_->world->removeAction(ptr);
		delete ptr;
	}

	auto PhysicsManager::Update(float timeDelta) -> void
	{
		bullet_->world->stepSimulation(timeDelta, 0);
		Game::Runtime::Instance()->Broadcast<Game::Event>(PhysicsComponent::TAG, EVT_PHYSICS_POST_UPDATE);
	}

	Game::ComponentTag<PhysicsComponent> const PhysicsComponent::TAG{ "PhysicsComponent" };
	PhysicsComponent::PhysicsComponent() :
		Component{ TAG },
		physicsManager_{},
		collisionObj_{},
		OnCOMDelete{}
	{
		Bind(EVT_PHYSICS_POST_UPDATE, &PhysicsComponent::OnPhysicsPostUpdate);
	}
	PhysicsComponent::PhysicsComponent(PhysicsComponent&& rhs) noexcept :
		Component(rhs),
		collisionObj_{ rhs.collisionObj_ },
		physicsManager_{ std::move(rhs.physicsManager_) },
		offset_{ std::move(rhs.offset_) },
		OnCOMDelete{ rhs.OnCOMDelete }
	{
		rhs.collisionObj_ = nullptr;
	}
	PhysicsComponent::~PhysicsComponent()
	{
		if (collisionObj_ != nullptr)
		{
			auto manager{ physicsManager_.lock() };
			if (manager == nullptr) return;
			(manager.get()->*OnCOMDelete)(collisionObj_);
			collisionObj_ = nullptr;
		}
	}
	auto PhysicsComponent::Clone() const -> Game::Component*
	{
		return nullptr;
	}
	auto PhysicsComponent::GetCharacterContoller() -> btKinematicCharacterController*
	{
		return collisionObj_;
	}
	auto PhysicsComponent::OnPhysicsPostUpdate(Game::Event const& evt) -> void
	{
		auto object{ GetObj().lock() };
		auto origin = this->collisionObj_->getGhostObject()->getWorldTransform().getOrigin();
		XMFLOAT3 newPosition{ origin.x() - offset_.x ,origin.y() - offset_.y, origin.z() - offset_.z };

		object->GetComponent<Game::TransformComponent>()->SetPosition(newPosition);
	}
}
