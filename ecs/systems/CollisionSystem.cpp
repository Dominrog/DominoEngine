#include "CollisionSystem.h"

void CollisionSystem::update(Registry& registry)
{
  auto view = registry.view<Transform, BoxCollider>();

  for (auto e : view)
    registry.get<BoxCollider>(e).colliding = false;

  for (auto itA = view.begin(); itA != view.end(); ++itA)
  {
    auto itB = itA;
    ++itB;

    for (; itB != view.end(); ++itB)
    {
      Entity a = *itA;
      Entity b = *itB;

      auto& ta = registry.get<Transform>(a);
      auto& ca = registry.get<BoxCollider>(a);

      auto& tb = registry.get<Transform>(b);
      auto& cb = registry.get<BoxCollider>(b);

      if ((ca.mask & cb.layer) == 0 || (cb.mask & ca.layer) == 0)
    		continue;


      if (aabbOverlap(ta, ca, tb, cb))
      {
        ca.colliding = true;
        cb.colliding = true;
      }
    }
  }
}


bool CollisionSystem::aabbOverlap(
    const Transform& ta, const BoxCollider& ca,
    const Transform& tb, const BoxCollider& cb)
{
    glm::vec3 halfA = ca.scale * ta.scale;
    glm::vec3 halfB = cb.scale * tb.scale;

    glm::vec3 posA = ta.position + ca.offset;
    glm::vec3 posB = tb.position + cb.offset;

    return
      abs(posA.x - posB.x) <= (halfA.x + halfB.x) &&
      abs(posA.y - posB.y) <= (halfA.y + halfB.y) &&
      abs(posA.z - posB.z) <= (halfA.z + halfB.z);
}
