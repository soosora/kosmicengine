#version 330 core

in float vHeight;
out vec4 FragColor;

void main() {
    // Normalize Y from [-1, 1] to [0, 1]
    float t = clamp((vHeight + 1.0) / 2.0, 0.0, 1.0);
    
    // Define colors: bottom (dark), horizon (intermediate) and top (blue)
    vec3 bottomColor = vec3(0.0, 0.0, 0.2);
    vec3 midColor    = vec3(0.5, 0.7, 0.9);
    vec3 topColor    = vec3(0.0, 0.5, 1.0);
    
    vec3 color;
    if (t < 0.5) {
        float blend = t * 2.0;
        color = mix(bottomColor, midColor, blend);
    } else {
        float blend = (t - 0.5) * 2.0;
        color = mix(midColor, topColor, blend);
    }
    
    FragColor = vec4(color, 1.0);
}
