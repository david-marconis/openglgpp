#version 450 core

in vec2 textureCoordinate;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    float ambientIntensity;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

    color = texture(textureSampler, textureCoordinate) * ambientColor;
}
