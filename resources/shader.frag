#version 450 core

in vec2 textureCoordinate;
in vec3 Normal;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;

void main() {
    vec4 lightColor = vec4(directionalLight.color, 1.0f);
    vec4 ambientColor = lightColor * directionalLight.ambientIntensity;
    
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = lightColor * directionalLight.diffuseIntensity * diffuseFactor;
    
    color = texture(textureSampler, textureCoordinate) * (ambientColor + diffuseColor);
}
