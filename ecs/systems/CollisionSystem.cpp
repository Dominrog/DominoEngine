#include "CollisionSystem.h"

CollisionInfo CollisionSystem::getCollisionInfo(const OBB& a, const OBB& b)
{
  CollisionInfo info;
  info.hasCollision = false;
  info.depth = std::numeric_limits<float>::max();
  
  std::array<glm::vec3, 15> axes;
  
  for(int i = 0; i < 3; i++) {
    axes[i] = a.axes[i];
  }
  
  for(int i = 0; i < 3; i++) {
    axes[3 + i] = b.axes[i];
  }
  
  int idx = 6;
  for(int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      axes[idx] = glm::cross(a.axes[i], b.axes[j]);
      float length = glm::length(axes[idx]);
      if(length < 0.0001f) continue;
      axes[idx] = glm::normalize(axes[idx]);
      idx++;
    }
  }
  
  for(int i = 0; i < idx; i++) {
    glm::vec3 axis = axes[i];
    
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
    float overlap = (projA + projB) - distance;
    
    if(overlap <= 0.0f) {
      return CollisionInfo{glm::vec3(0.0f), glm::vec3(0.0f), 0.0f, false};
    }
    
    if(overlap < info.depth) {
      info.depth = overlap;
      info.normal = axis;
      
      if(glm::dot(centerDiff, axis) < 0.0f) {
        info.normal = -axis;
      }
    }
  }
  
  info.hasCollision = true;
  
  glm::vec3 pointOnA = a.center;
  glm::vec3 pointOnB = b.center;
  
  float distA = glm::dot(pointOnA, info.normal);
  float distB = glm::dot(pointOnB, info.normal);
  
  float t = (distB - distA + info.depth) / (2.0f * info.depth);
  info.point = pointOnA + (pointOnB - pointOnA) * t;
  
  return info;
}

void CollisionSystem::update(Registry& registry, float delta_time)
{
  auto view = registry.view<Transform, BoxCollider, Physics>();

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
      auto& pa = registry.get<Physics>(a);

      auto& tb = registry.get<Transform>(b);
      auto& cb = registry.get<BoxCollider>(b);
      auto& pb = registry.get<Physics>(b);

      if ((ca.mask & cb.layer) == 0 || (cb.mask & ca.layer) == 0)
          continue;

      glm::mat3 rotA = glm::mat3_cast(ta.rotationQ);
      glm::mat3 rotB = glm::mat3_cast(tb.rotationQ);
      OBB obbA, obbB;

      obbA.center = ta.position;
      obbB.center = tb.position;

      ca.scale = ta.scale;
      cb.scale = tb.scale;
      obbA.scale = ca.scale; 
      obbB.scale = cb.scale;

      obbA.axes[0] = glm::normalize(rotA[0]);
      obbA.axes[1] = glm::normalize(rotA[1]);
      obbA.axes[2] = glm::normalize(rotA[2]);

      obbB.axes[0] = glm::normalize(rotB[0]);
      obbB.axes[1] = glm::normalize(rotB[1]);
      obbB.axes[2] = glm::normalize(rotB[2]);

      AABB aabbA = fittingAABB(obbA);
      AABB aabbB = fittingAABB(obbB);

      if (!aabbOverlap(aabbA, aabbB))
      {
        ca.colliding = false;
        cb.colliding = false;
        continue;
      }
      
      CollisionInfo collision = getCollisionInfo(obbA, obbB);
      
      if (collision.hasCollision)
      {
        ca.colliding = true;
        cb.colliding = true;

        if (ca.collidable && cb.collidable)
        {
          glm::vec3 n = collision.normal;
          glm::vec3 relativeVel = pb.velocity - pa.velocity;
          float velAlongNormal = glm::dot(relativeVel, n);

          if (velAlongNormal > 0.0f) continue;

          float e = std::min(pa.restitution, pb.restitution);

          float j = -(1.0f + e) * velAlongNormal;
          j /= pa.invMass + pb.invMass;

          glm::vec3 impulse = j * n;
          
          if (pa.invMass > 0.0f) {
            pa.velocity -= impulse * pa.invMass;
          }
          if (pb.invMass > 0.0f) {
            pb.velocity += impulse * pb.invMass;
          }
          
          if (collision.depth > 0.0f) {
            const float slop = 0.001f;
            const float percent = 0.3f;
            
            float correction = std::max(collision.depth - slop, 0.0f) / (pa.invMass + pb.invMass) * percent;
            
            glm::vec3 correctionVec = correction * n;
            
            if (pa.invMass > 0.0f) {
              ta.position -= correctionVec * pa.invMass;
            }
            if (pb.invMass > 0.0f) {
              tb.position += correctionVec * pb.invMass;
            }
          }
        }
      }
      else 
      {
        ca.colliding = false;
        cb.colliding = false;
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