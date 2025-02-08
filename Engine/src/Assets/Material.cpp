#include "Kosmic/Assets/Material.hpp"

namespace Kosmic::Assets {

Material::Material() = default;

void Material::SetDiffuseMap(const std::string& path) {
    diffuseMap = std::make_shared<Renderer::Texture>(path);
}

void Material::SetSpecularMap(const std::string& path) {
    specularMap = std::make_shared<Renderer::Texture>(path);
}

void Material::SetNormalMap(const std::string& path) {
    normalMap = std::make_shared<Renderer::Texture>(path);
}

} // namespace Kosmic::Assets
