#version 330 core

in vec3 vertexColor;
in vec2 TexCoord;
in vec3 Normal;
out vec4 FragColor;

uniform sampler2D u_Texture;
uniform vec4 u_Color;

// Ambient light uniforms
uniform vec3 u_AmbientLightColor = vec3(1.0, 1.0, 1.0);
uniform float u_AmbientLightIntensity = 0.7;
// Directional light uniforms
uniform vec3 u_DirLightDirection = vec3(-0.2, -1.0, -0.3);
uniform vec3 u_DirLightColor = vec3(1.0, 1.0, 1.0);
uniform float u_DirLightIntensity = 0.3;

void main() {
    vec3 ambient = u_AmbientLightColor * u_AmbientLightIntensity;
    vec3 normal = normalize(Normal);
    float diff = max(dot(normal, normalize(-u_DirLightDirection)), 0.0);
    vec3 diffuse = u_DirLightColor * u_DirLightIntensity * diff;
    
    // Combine color from texture and uniform
    vec4 finalColor = u_Color; // use only uniform color
    if (textureSize(u_Texture, 0).x > 1) {
        finalColor *= texture(u_Texture, TexCoord);
    }
    
    // Apply lighting
    finalColor.rgb *= (ambient + diffuse);
    FragColor = finalColor;
}
