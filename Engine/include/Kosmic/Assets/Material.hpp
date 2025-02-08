#pragma once
#include "Kosmic/Core/Math/Math.hpp"
#include "Kosmic/Renderer/Texture.hpp"
#include <memory>

namespace Kosmic::Assets {

class Material {
public:
    Material();
    ~Material() = default;

    // Material properties
    Math::Vector3 ambient{1.0f, 1.0f, 1.0f};
    Math::Vector3 diffuse{1.0f, 1.0f, 1.0f};
    Math::Vector3 specular{1.0f, 1.0f, 1.0f};
    float shininess{32.0f};

    // Textures
    std::shared_ptr<Renderer::Texture> diffuseMap;
    std::shared_ptr<Renderer::Texture> specularMap;
    std::shared_ptr<Renderer::Texture> normalMap;

    // Set texture methods
    void SetDiffuseMap(const std::string& path);
    void SetSpecularMap(const std::string& path);
    void SetNormalMap(const std::string& path);
};

} // namespace Kosmic::Assets
