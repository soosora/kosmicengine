#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;

out float vHeight; // Pass Y coordinate for gradient

void main() {
    mat4 viewNoTranslate = view;
    viewNoTranslate[3] = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 pos = projection * viewNoTranslate * vec4(aPos, 1.0);
    gl_Position = pos;
    vHeight = aPos.y; // Use Y for gradient computation
}
