#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <Jolt/Physics/Body/MotionType.h>

struct RigidBody {
    JPH::BodyID bodyID;

    JPH::EMotionType motionType = JPH::EMotionType::Dynamic;

    float mass = 1.0f;
    float friction = 0.5f;
    float restitution = 0.1f;
};