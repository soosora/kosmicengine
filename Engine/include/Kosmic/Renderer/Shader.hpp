#pragma once
#include "Kosmic/Core/Math/Math.hpp"
#include <string>
#include <memory>

// Forward declaration of the GLuint type
typedef unsigned int GLuint;
typedef unsigned int GLenum;

namespace Kosmic::Renderer {

class Shader {
public:
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    static std::shared_ptr<Shader> CreateBasicShader();

    void SetMat4(const std::string& name, const Math::Mat4& matrix);
    void SetVec3(const std::string& name, const Math::Vector3& value);
    void SetFloat(const std::string& name, float value);
    void SetInt(const std::string& name, int value);

private:
    GLuint m_ShaderID;
    static GLuint CompileShader(GLenum type, const std::string& source);
    static GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};

} // namespace Kosmic::Renderer
