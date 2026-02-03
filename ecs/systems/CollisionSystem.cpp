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

      glm::mat3 rotA = glm::mat3_cast(ta.rotation);
      glm::mat3 rotB = glm::mat3_cast(tb.rotation);
      OBB obbA, obbB;

      obbA.center = ta.position;
      obbB.center = tb.position;

      obbA.scale = ta.scale;
      obbB.scale = tb.scale;

      obbA.axes[0] = glm::normalize(rotA[0]);
      obbA.axes[1] = glm::normalize(rotA[1]);
      obbA.axes[2] = glm::normalize(rotA[2]);

      obbB.axes[0] = glm::normalize(rotB[0]);
      obbB.axes[1] = glm::normalize(rotB[1]);
      obbB.axes[2] = glm::normalize(rotB[2]);

      AABB aabbA = fittingAABB(obbA);
      AABB aabbB = fittingAABB(obbB);


      if (aabbOverlap(aabbA, aabbB) && obbOverlap(obbA, obbB))
      {
        ca.colliding = true;
        cb.colliding = true;
      }
    }
  }
}


bool CollisionSystem::aabbOverlap(const AABB& a, const AABB& b)
{
  return  (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
          (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
          (a.min.z <= b.max.z && a.max.z >= b.min.z);
}


AABB CollisionSystem::fittingAABB(const OBB& obb)
{
  glm::mat3 rot = glm::mat3(obb.axes[0], obb.axes[1], obb.axes[2]);
  rot[0] = glm::abs(rot[0]);
  rot[1] = glm::abs(rot[1]);
  rot[2] = glm::abs(rot[2]);

  glm::vec3 extent = rot * obb.scale;

  return AABB{
    obb.center - extent,
    obb.center + extent
  };
}


bool CollisionSystem::obbOverlap(const OBB& a, const OBB& b)
{
  for(int i = 0; i < 3; i++) {
        if(!overlapOnAxis(a.axes[i], a, b)) return false;
        if(!overlapOnAxis(b.axes[i], a, b)) return false;
    }
    
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      glm::vec3 axis = glm::cross(a.axes[i], b.axes[j]);
      
      float length2 = glm::length(axis);
      if(length2 < 0.0001f) continue;
      
      axis = glm::normalize(axis);
      if(!overlapOnAxis(axis, a, b)) return false;
    }
  }
  
  return true;
}

bool CollisionSystem::overlapOnAxis(const glm::vec3& axis, const OBB& a, const OBB& b)
{
  float projA = 
    a.scale.x * std::abs(glm::dot(a.axes[0], axis)) +
    a.scale.y * std::abs(glm::dot(a.axes[1], axis)) +
    a.scale.z * std::abs(glm::dot(a.axes[2], axis));
  
  float projB = 
    b.scale.x * std::abs(glm::dot(b.axes[0], axis)) +
    b.scale.y * std::abs(glm::dot(b.axes[1], axis)) +
    b.scale.z * std::abs(glm::dot(b.axes[2], axis));
  
  glm::vec3 centerDiff = b.center - a.center;
  float distance = std::abs(glm::dot(centerDiff, axis));
  
  return distance <= (projA + projB);
}