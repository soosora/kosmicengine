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
