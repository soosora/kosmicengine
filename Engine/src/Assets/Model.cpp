#include "Kosmic/Assets/Model.hpp"
#include "Kosmic/Core/Logging.hpp"
#include <filesystem>

namespace Kosmic::Assets {

Model::Model(const std::string& path) {
    LoadModel(path);
}

void Model::LoadModel(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, 
        aiProcess_Triangulate | 
        aiProcess_GenNormals | 
        aiProcess_FlipUVs |
        aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        KOSMIC_ERROR("ASSIMP ERROR: {}", importer.GetErrorString());
        return;
    }

    m_Directory = std::filesystem::path(path).parent_path().string();
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    // Process meshes in current node
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_Meshes.push_back(ProcessMesh(mesh, scene));
        
        // Process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            m_Materials.push_back(ProcessMaterial(material, scene));
        }
    }

    // Process child nodes
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

std::shared_ptr<Renderer::Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Renderer::Vertex> vertices;
    std::vector<uint32_t> indices;

    // Process vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Renderer::Vertex vertex;
        
        // > Convert Assimp positions to mesh vertices
        vertex.Position = {
            mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z
        };

        if(mesh->mNormals)
            vertex.Normal = {
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            };

        if(mesh->mTextureCoords[0])
            vertex.TexCoords = {
                mesh->mTextureCoords[0][i].x,
                mesh->mTextureCoords[0][i].y
            };

        vertex.Color = {1.0f, 1.0f, 1.0f};
        vertices.push_back(vertex);
    }

    // Process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return std::make_shared<Renderer::Mesh>(vertices, indices);
}

std::shared_ptr<Material> Model::ProcessMaterial(aiMaterial* material, const aiScene* scene) {
    auto mat = std::make_shared<Material>();

    aiColor3D color(0.f);
    float shininess;

    if(material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
        mat->ambient = {color.r, color.g, color.b};
    if(material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
        mat->diffuse = {color.r, color.g, color.b};
    if(material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
        mat->specular = {color.r, color.g, color.b};
    if(material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
        mat->shininess = shininess;

    // Load textures
    aiString str;
    if(material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
        material->GetTexture(aiTextureType_DIFFUSE, 0, &str);
        mat->SetDiffuseMap(m_Directory + "/" + str.C_Str());
    }

    return mat;
}

void Model::Draw(const std::shared_ptr<Renderer::Shader>& shader) {
    shader->Bind();
    // Draw each mesh with its material
    for(size_t i = 0; i < m_Meshes.size(); i++) {
        // Set model matrix for this mesh
        shader->SetMat4("model", m_Meshes[i]->GetTransform());
        
        if(i < m_Materials.size() && m_Materials[i]->diffuseMap)
            m_Materials[i]->diffuseMap->Bind(0);
            
        m_Meshes[i]->Draw();
        
        if(i < m_Materials.size() && m_Materials[i]->diffuseMap)
            m_Materials[i]->diffuseMap->Unbind();
    }
    shader->Unbind();
}

} // namespace Kosmic::Assets
