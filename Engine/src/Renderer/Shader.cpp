#include <GL/glew.h>
#include "Kosmic/Renderer/Shader.hpp"
#include <SDL2/SDL_opengl.h>
#include "Kosmic/Core/Logging.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

namespace {

// Helper function to read shader source from file
std::string LoadShaderSource(const std::string& filePath) {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        KOSMIC_ERROR("Failed to open shader file: {}", filePath);
        return "";
    }
    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

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
        KOSMIC_ERROR("Error compiling shader: {}", buffer);
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
        KOSMIC_ERROR("Error linking shader program: {}", buffer);
    }
    return program;
}

void Shader::SetMat4(const std::string& name, const Math::Mat4& matrix) {
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::SetVec3(const std::string& name, const Math::Vector3& value) {
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform3f(location, value.x, value.y, value.z);
}

void Shader::SetFloat(const std::string& name, float value) {
    GLint location = glGetUniformLocation(m_ShaderID, name.c_str());
    glUniform1f(location, value);
}

void Shader::SetInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(m_ShaderID, name.c_str()), value);
}

std::shared_ptr<Shader> Shader::CreateBasicShader() {
    std::string vertexPath   = "Resources/Shaders/basic.vert";
    std::string fragmentPath = "Resources/Shaders/basic.frag";
    std::string vertexSrc = LoadShaderSource(vertexPath);
    std::string fragmentSrc = LoadShaderSource(fragmentPath);
    return std::make_shared<Shader>(vertexSrc, fragmentSrc);
}

} // namespace Kosmic::Renderer
