#pragma once

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

private:
    GLuint m_ShaderID;
    GLuint CompileShader(GLenum type, const std::string& source);
    GLuint LinkProgram(GLuint vertexShader, GLuint fragmentShader);
};

} // namespace Kosmic::Renderer
