#include "PhysicsSystem.h"

#include <Jolt/Jolt.h>
#include <Jolt/RegisterTypes.h>

#include <Jolt/Core/Factory.h>
#include <Jolt/Core/TempAllocator.h>
#include <Jolt/Core/JobSystemThreadPool.h>

#include <Jolt/Physics/PhysicsSystem.h>
#include <Jolt/Physics/Body/Body.h>
#include <Jolt/Physics/Body/BodyCreationSettings.h>

#include <Jolt/Physics/Collision/ObjectLayerPairFilterTable.h>
#include <Jolt/Physics/Collision/BroadPhase/BroadPhaseLayerInterfaceTable.h>
#include <Jolt/Physics/Collision/BroadPhase/ObjectVsBroadPhaseLayerFilterTable.h>

#include <glm/gtc/quaternion.hpp>

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <stdexcept>
#include <thread>

#include "transform.h"
#include "collider.h"
#include "rigidbody.h"

namespace {
  constexpr JPH::ObjectLayer STATIC_LAYER = 0;
  constexpr JPH::ObjectLayer MOVING_LAYER = 1;

  constexpr float FIXED_STEP = 1.0f / 60.0f;

  void traceJolt(const char* format, ...)
  {
    va_list arguments;
    va_start(arguments, format);
    std::vfprintf(stderr, format, arguments);
    va_end(arguments);

    std::fputc('\n', stderr);
}

#ifdef JPH_ENABLE_ASSERTS
bool assertJolt(
    const char* expression,
    const char* message,
    const char* file,
    JPH::uint line)
{
  std::fprintf(
    stderr,
    "Jolt assertion: %s (%s:%u) %s\n",
    expression,
    file,
    line,
    message ? message : ""
  );

  return true;
}
#endif
}

struct PhysicsSystem::Impl {
  std::unique_ptr<JPH::BroadPhaseLayerInterfaceTable> broadPhase;
  std::unique_ptr<JPH::ObjectLayerPairFilterTable> pairFilter;
  std::unique_ptr<JPH::ObjectVsBroadPhaseLayerFilterTable> broadPhaseFilter;

  std::unique_ptr<JPH::TempAllocatorImpl> tempAllocator;
  std::unique_ptr<JPH::JobSystemThreadPool> jobs;
  std::unique_ptr<JPH::PhysicsSystem> world;

  bool ownsJolt = false;

  ~Impl()
  {
    world.reset();
    jobs.reset();
    tempAllocator.reset();

    broadPhaseFilter.reset();
    pairFilter.reset();
    broadPhase.reset();

    if (ownsJolt) {
      JPH::UnregisterTypes();

      delete JPH::Factory::sInstance;
      JPH::Factory::sInstance = nullptr;
    }
  }
};

PhysicsSystem::PhysicsSystem() = default;
PhysicsSystem::~PhysicsSystem() = default;

JPH::BodyInterface& PhysicsSystem::getBodyInterface()
{
  if (!impl)
    throw std::runtime_error("PhysicsSystem::init() fehlt");

  return impl->world->GetBodyInterface();
}

void PhysicsSystem::init()
{
  if (impl) return;

  if (JPH::Factory::sInstance != nullptr)
    throw std::runtime_error("Jolt ist bereits initialisiert");

  JPH::RegisterDefaultAllocator();

  JPH::Trace = traceJolt;

#ifdef JPH_ENABLE_ASSERTS
  JPH::AssertFailed = assertJolt;
#endif

  impl = std::make_unique<Impl>();

  JPH::Factory::sInstance = new JPH::Factory();
  impl->ownsJolt = true;

  JPH::RegisterTypes();

  impl->tempAllocator =
    std::make_unique<JPH::TempAllocatorImpl>(10 * 1024 * 1024);

  unsigned int cores = std::thread::hardware_concurrency();

  int workerCount = cores > 1
    ? static_cast<int>(cores - 1)
    : 1;

  impl->jobs = std::make_unique<JPH::JobSystemThreadPool>(
    JPH::cMaxPhysicsJobs,
    JPH::cMaxPhysicsBarriers,
    workerCount
  );

  impl->broadPhase =
    std::make_unique<JPH::BroadPhaseLayerInterfaceTable>(2, 2);

  impl->broadPhase->MapObjectToBroadPhaseLayer(
    STATIC_LAYER,
    JPH::BroadPhaseLayer(0)
  );

  impl->broadPhase->MapObjectToBroadPhaseLayer(
    MOVING_LAYER,
    JPH::BroadPhaseLayer(1)
  );

  impl->pairFilter =
    std::make_unique<JPH::ObjectLayerPairFilterTable>(2);

  impl->pairFilter->EnableCollision(STATIC_LAYER, MOVING_LAYER);
  impl->pairFilter->EnableCollision(MOVING_LAYER, MOVING_LAYER);

  impl->broadPhaseFilter =
	  std::make_unique<JPH::ObjectVsBroadPhaseLayerFilterTable>(
	    *impl->broadPhase,
	    2,
	    *impl->pairFilter,
	    2
		);

  impl->world = std::make_unique<JPH::PhysicsSystem>();

  impl->world->Init(
    10240,  // Maximale Anzahl Körper
    0,      // Standardanzahl Body-Mutexe
    65536,  // Maximale Anzahl Body-Paare
    10240,  // Maximale Anzahl Kontaktbedingungen
    *impl->broadPhase,
    *impl->broadPhaseFilter,
    *impl->pairFilter
  );

  impl->world->SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));

  accumulator = 0.0f;
}

void PhysicsSystem::createBody(Registry& registry, Entity entity)
{
  if (!impl)
    throw std::runtime_error("PhysicsSystem::init() fehlt");

  auto& transform = registry.get<Transform>(entity);
  auto& collider = registry.get<Collider>(entity);
  auto& rigidBody = registry.get<RigidBody>(entity);

  if (!rigidBody.bodyID.IsInvalid())
    return;

  if (collider.shape.GetPtr() == nullptr)
    throw std::runtime_error("Collider hat keine Shape");

  if (rigidBody.motionType == JPH::EMotionType::Dynamic && rigidBody.mass <= 0.0f)
  {
    throw std::runtime_error("Dynamischer Koerper braucht positive Masse");
  }

  glm::quat rotation = glm::normalize(transform.rotationQ);

  JPH::ObjectLayer layer =
    rigidBody.motionType == JPH::EMotionType::Static
    ? STATIC_LAYER
    : MOVING_LAYER;

  JPH::BodyCreationSettings settings(
    collider.shape.GetPtr(),

    JPH::RVec3(
      transform.position.x,
      transform.position.y,
      transform.position.z
    ),

    JPH::Quat(
      rotation.x,
      rotation.y,
      rotation.z,
      rotation.w
    ),

    rigidBody.motionType,
    layer
  );

  settings.mFriction = rigidBody.friction;
  settings.mRestitution = rigidBody.restitution;
  settings.mIsSensor = collider.trigger;
  settings.mUserData = static_cast<JPH::uint64>(entity);

  if (rigidBody.motionType == JPH::EMotionType::Dynamic)
  {
    settings.mOverrideMassProperties =
      JPH::EOverrideMassProperties::CalculateInertia;

    settings.mMassPropertiesOverride.mMass = rigidBody.mass;
  }

  auto& bodies = impl->world->GetBodyInterface();

  JPH::Body* body = bodies.CreateBody(settings);

  if (body == nullptr)
    throw std::runtime_error("Jolt konnte keinen Koerper erstellen");

  rigidBody.bodyID = body->GetID();

  JPH::EActivation activation =
    rigidBody.motionType == JPH::EMotionType::Static
    ? JPH::EActivation::DontActivate
    : JPH::EActivation::Activate;

  bodies.AddBody(rigidBody.bodyID, activation);
}

void PhysicsSystem::removeBody(Registry& registry, Entity entity)
{
  if (!impl)
    return;

  auto& rigidBody = registry.get<RigidBody>(entity);

  if (rigidBody.bodyID.IsInvalid())
    return;

  auto& bodies = impl->world->GetBodyInterface();

  bodies.RemoveBody(rigidBody.bodyID);
  bodies.DestroyBody(rigidBody.bodyID);

  rigidBody.bodyID = JPH::BodyID();
}

void PhysicsSystem::update(Registry& registry, float deltaTime)
{
  if (!impl)
    return;

  accumulator += std::clamp(deltaTime, 0.0f, 0.25f);

  while (accumulator >= FIXED_STEP)
  {
    impl->world->Update(
      FIXED_STEP,
      1,
      impl->tempAllocator.get(),
      impl->jobs.get()
    );

    accumulator -= FIXED_STEP;
  }

  auto& bodies = impl->world->GetBodyInterface();

  for (Entity entity : registry.view<Transform, RigidBody>())
  {
    auto& transform = registry.get<Transform>(entity);
    auto& rigidBody = registry.get<RigidBody>(entity);

    if (rigidBody.bodyID.IsInvalid())
      continue;

    JPH::RVec3 position;
    JPH::Quat rotation;

    bodies.GetPositionAndRotation(
      rigidBody.bodyID,
      position,
      rotation
    );

    transform.position = glm::vec3(
      static_cast<float>(position.GetX()),
      static_cast<float>(position.GetY()),
      static_cast<float>(position.GetZ())
    );

    transform.rotationQ = glm::quat(
      rotation.GetW(),
      rotation.GetX(),
      rotation.GetY(),
      rotation.GetZ()
    );
  }
}

void PhysicsSystem::shutdown(Registry& registry)
{
  if (!impl)
    return;

  for (Entity entity : registry.view<RigidBody>())
    removeBody(registry, entity);

  for (Entity entity : registry.view<Collider>())
    registry.get<Collider>(entity).shape = nullptr;

  impl.reset();
  accumulator = 0.0f;
}