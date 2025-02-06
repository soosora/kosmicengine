#pragma once

#include "Kosmic/Core/Math/Math.hpp"
#include <vector>
#include <memory>

namespace Kosmic::Renderer {

struct Vertex {
    Math::Vector3 Position;
    Math::Vector3 Normal;
    Math::Vector2 TexCoords;
    Math::Vector3 Color;
};

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    void Draw() const;

    // Add transform support
    void SetTransform(const Math::Mat4& transform);

    const Math::Mat4& GetTransform() const;

private:
    void SetupMesh();

    uint32_t m_VAO, m_VBO, m_EBO;
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;

    Math::Mat4 m_Transform{1.0f}; // Identity matrix
};

} // namespace Kosmic::Renderer

namespace Kosmic::Renderer::MeshLibrary {
    std::shared_ptr<Mesh> Cube();
    std::shared_ptr<Mesh> Triangle();
    std::shared_ptr<Mesh> Quad();
} // namespace Kosmic::Renderer::MeshLibrary
