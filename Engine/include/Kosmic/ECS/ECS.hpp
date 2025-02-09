#ifndef KOSMIC_ECS_HPP
#define KOSMIC_ECS_HPP

#include "entt/entt.hpp"
#include "Kosmic/Core/Math/Math.hpp"

namespace Kosmic {
  namespace ECS {

    // ECS manager for handling entities and components using EnTT
    class ECSManager {
    public:
      static entt::registry& GetRegistry() {
        static entt::registry registry;
        return registry;
      }
    };

    // Basic Transform component for entities
    struct Transform {
      Math::Vector3 position;
      Math::Vector3 rotation;
      Math::Vector3 scale;
      Transform() 
        : position{0.0f, 0.0f, 0.0f}, rotation{0.0f, 0.0f, 0.0f}, scale{1.0f, 1.0f, 1.0f} {}
    };

  } // namespace ECS
} // namespace Kosmic

#endif // KOSMIC_ECS_HPP
