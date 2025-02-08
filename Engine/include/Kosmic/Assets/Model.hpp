#pragma once

#include "Kosmic/Renderer/Mesh.hpp"
#include "Kosmic/Assets/Material.hpp"
#include "Kosmic/Renderer/Shader.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <vector>
#include <memory>

namespace Kosmic::Assets {

class Model {
public:
    Model(const std::string& path);
    ~Model() = default;

    // Draws the model using the provided shader
    void Draw(const std::shared_ptr<Renderer::Shader>& shader);
    const std::vector<std::shared_ptr<Renderer::Mesh>>& GetMeshes() const { return m_Meshes; }
    const std::vector<std::shared_ptr<Material>>& GetMaterials() const { return m_Materials; }

private:
    void LoadModel(const std::string& path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Renderer::Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::shared_ptr<Material> ProcessMaterial(aiMaterial* material, const aiScene* scene);
    
    std::vector<std::shared_ptr<Renderer::Mesh>> m_Meshes;
    std::vector<std::shared_ptr<Material>> m_Materials;
    std::string m_Directory;
};

} // namespace Kosmic::Assets
