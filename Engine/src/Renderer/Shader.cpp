#include <GL/glew.h>
#include "Kosmic/Renderer/Shader.hpp"
#include <SDL2/SDL_opengl.h>
#include <iostream>

namespace {
const char* basicVertexShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 vertexColor;
    void main() {
        gl_Position = projection * view * vec4(aPos, 1.0);
        vertexColor = aColor;
    }
)";

const char* basicFragmentShader = R"(
    #version 330 core
    in vec3 vertexColor;
    out vec4 FragColor;
    void main() {
        FragColor = vec4(vertexColor, 1.0);
    }
)";
}

namespace Kosmic::Renderer {

Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) {
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSrc);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSrc);
    m_ShaderID = LinkProgram(vertexShader, fragmentShader);

    // After linking the shaders, delete the objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(m_ShaderID);
}

void Shader::Bind() const {
    glUseProgram(m_ShaderID);
}

void Shader::Unbind() const {
    glUseProgram(0);
}

GLuint Shader::CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    // Error checking
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        std::cerr << "Error compiling shader: " << buffer << std::endl;
    }
    return shader;
}

GLuint Shader::LinkProgram(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Link error checking
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetProgramInfoLog(program, 512, nullptr, buffer);
        std::cerr << "Error linking shader program: " << buffer << std::endl;
    }
    return program;
}

void Shader::SetMat4(const std::string& name, const Math::Mat4& matrix) {
    // Obtain uniform location and set the matrix
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

std::shared_ptr<Shader> Shader::CreateBasicShader() {
    return std::make_shared<Shader>(basicVertexShader, basicFragmentShader);
}

} // namespace Kosmic::Renderer
