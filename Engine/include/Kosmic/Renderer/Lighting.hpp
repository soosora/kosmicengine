#pragma once

#include "Kosmic/Core/Math/Math.hpp"

namespace Kosmic::Renderer::Lighting {

// Ambient light structure
struct AmbientLight {
    Math::Vector3 color;
    float intensity;
};

// Directional light structure
struct DirectionalLight {
    Math::Vector3 direction;
    Math::Vector3 color;
    float intensity;
};

// Point light structure
struct PointLight {
    Math::Vector3 position;
    Math::Vector3 color;
    float intensity;
    float range;
};

// Spot light structure
struct SpotLight {
    Math::Vector3 position;
    Math::Vector3 direction;
    Math::Vector3 color;
    float intensity;
    float cutoff;
    float outerCutoff;
};

} // namespace Kosmic::Renderer::Lighting
