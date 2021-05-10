#version 450 core

in vec2 textureCoordinate;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight {
    vec3 color;
    vec3 direction;
    float ambientIntensity;
    float diffuseIntensity;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform sampler2D textureSampler;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

void main() {
    vec4 ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }

    color = texture(textureSampler, textureCoordinate) * (ambientColor + diffuseColor + specularColor);
}
