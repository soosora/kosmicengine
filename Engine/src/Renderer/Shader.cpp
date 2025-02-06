#include <GL/glew.h>
#include "Kosmic/Renderer/Shader.hpp"
#include <SDL2/SDL_opengl.h>
#include "Kosmic/Core/Logging.hpp"
#include <iostream>

namespace {

const char* basicVertexShader = R"(
    #version 330 core
    
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;
    layout (location = 3) in vec3 aNormal;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    out vec3 vertexColor;
    out vec2 TexCoord;
    out vec3 Normal;

    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        vertexColor = aColor;
        TexCoord = aTexCoord;
        // Passing normal directly
        Normal = aNormal;
    }
)";

const char* basicFragmentShader = R"(
    #version 330 core

    in vec3 vertexColor;
    in vec2 TexCoord;
    in vec3 Normal;
    out vec4 FragColor;
    uniform sampler2D u_Texture;
    // Ambient light uniforms
    uniform vec3 u_AmbientLightColor;
    uniform float u_AmbientLightIntensity;
    // Directional light uniforms
    uniform vec3 u_DirLightDirection;
    uniform vec3 u_DirLightColor;
    uniform float u_DirLightIntensity;

    void main() {
        vec4 texColor = texture(u_Texture, TexCoord);
        vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
        // Compute diffuse lighting using Lambert's law
        float diff = max(dot(normalize(Normal), normalize(-u_DirLightDirection)), 0.0);
        vec3 diffuse = u_DirLightColor * u_DirLightIntensity * diff;
        FragColor = texColor * vec4(vertexColor * (ambient + diffuse), 1.0);
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
    return std::make_shared<Shader>(basicVertexShader, basicFragmentShader);
}

} // namespace Kosmic::Renderer
