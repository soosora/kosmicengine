#include "Kosmic/Renderer/Mesh.hpp"
#include <GL/glew.h>

namespace Kosmic::Renderer {

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
    : m_Vertices(vertices), m_Indices(indices) {
    SetupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), m_Indices.data(), GL_STATIC_DRAW);

    // Configure vertex attributes
    // Position -> layout(location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    // Color -> layout(location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // TexCoords -> layout(location = 2)
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    // Normal -> layout(location = 3)
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    
    glBindVertexArray(0);
}

void Mesh::Bind() const {
    glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const {
    glBindVertexArray(0);
}

void Mesh::Draw() const {
    Bind();
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
    Unbind();
}

void Mesh::SetTransform(const Math::Mat4& transform) {
    m_Transform = transform;
}

const Math::Mat4& Mesh::GetTransform() const {
    return m_Transform;
}

} // namespace Kosmic::Renderer

namespace Kosmic::Renderer::MeshLibrary {

std::shared_ptr<Mesh> Cube() {
    using namespace Math;
    std::vector<Vertex> vertices = {
        // Front face
        {{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        // Back face
        {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}},
        {{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}, {1.0f, 1.0f, 1.0f}},
        {{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}, {1.0f, 1.0f, 1.0f}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,  // Front
        4, 5, 6, 6, 7, 4,  // Back
        3, 2, 6, 6, 5, 3,  // Top
        0, 4, 7, 7, 1, 0,  // Bottom
        1, 7, 6, 6, 2, 1,  // Right
        4, 0, 3, 3, 5, 4   // Left
    };

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> Triangle() {
    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,  // Front
        4, 5, 6, 6, 7, 4,  // Back
        3, 2, 6, 6, 5, 3,  // Top
        0, 4, 7, 7, 1, 0,  // Bottom
        1, 7, 6, 6, 2, 1,  // Right
        4, 0, 3, 3, 5, 4   // Left
    };

    return std::make_shared<Mesh>(vertices, indices);
}

std::shared_ptr<Mesh> Sphere() {
    const unsigned int sectorCount = 36;
    const unsigned int stackCount = 18;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    float x, y, z, xy;
    float nx, ny, nz, lengthInv = 1.0f;
    float s, t;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackAngle = Math::PI / 2 - i * Math::PI / stackCount;
        xy = cosf(stackAngle);
        z = sinf(stackAngle);

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            float sectorAngle = j * 2 * Math::PI / sectorCount;
            x = xy * cosf(sectorAngle);
            y = xy * sinf(sectorAngle);
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;
            s = static_cast<float>(j) / sectorCount;
            t = static_cast<float>(i) / stackCount;

            Vertex vert;
            vert.Position = { x, y, z };
            vert.Color = { 1.0f, 1.0f, 1.0f };
            vert.TexCoords = { s, t };
            vert.Normal = { nx, ny, nz };
            vertices.push_back(vert);
        }
    }

    for (unsigned int i = 0; i < stackCount; ++i) {
        unsigned int k1 = i * (sectorCount + 1);
        unsigned int k2 = k1 + sectorCount + 1;

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }

    return std::make_shared<Mesh>(vertices, indices);
}

} // namespace Kosmic::Renderer
