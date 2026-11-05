#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyInterface.h>

#include <memory>

#include "registry.h"

class PhysicsSystem {
public:
    PhysicsSystem();
    ~PhysicsSystem();

    PhysicsSystem(const PhysicsSystem&) = delete;
    PhysicsSystem& operator=(const PhysicsSystem&) = delete;

    JPH::BodyInterface& getBodyInterface();
    
    void init();

    void createBody(Registry& registry, Entity entity);
    void removeBody(Registry& registry, Entity entity);

    void update(Registry& registry, float deltaTime);

    void shutdown(Registry& registry);

private:
    struct Impl;
    std::unique_ptr<Impl> impl;

    float accumulator = 0.0f;
};