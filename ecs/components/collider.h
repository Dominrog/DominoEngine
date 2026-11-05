#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Collision/Shape/Shape.h>

struct Collider {
    JPH::RefConst<JPH::Shape> shape;

    bool trigger = false;
};