#version 450 core

in vec2 textureCoordinate;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light {
    vec3 color;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 direction;
};

struct PointLight {
    Light base;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    PointLight pointLight;
    vec3 direction;
    float edge;
};

struct Material {
    float specularIntensity;
    float shininess;
};

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D textureSampler;
uniform Material material;

uniform vec3 eyePosition;

vec4 calculateLightByDirection(Light light, vec3 direction) {
    vec4 ambientColor = vec4(light.color, 1.0f) * light.ambientIntensity;
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseColor = vec4(light.color, 1.0f) * light.diffuseIntensity * diffuseFactor;

    vec4 specularColor = vec4(0, 0, 0, 0);

    if(diffuseFactor > 0.0f) {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(direction, normalize(Normal)));

        float specularFactor = dot(fragToEye, reflectedVertex);
        if(specularFactor > 0.0f) {
            specularFactor = pow(specularFactor, material.shininess);
            specularColor = vec4(light.color * material.specularIntensity * specularFactor, 1.0f);
        }
    }
    return ambientColor + diffuseColor + specularColor;
}

vec4 calculateDirectionalLight() {
    return calculateLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 calculatePointLight(PointLight pointLight) {
    vec3 direction = FragPos - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 color = calculateLightByDirection(pointLight.base, direction);
    float attenuation = pointLight.quadratic * distance * distance +
        pointLight.linear * distance +
        pointLight.constant;
    return color / attenuation;
}

vec4 calculateSpotLight(SpotLight spotLight) {
    vec3 rayDirection = normalize(FragPos - spotLight.pointLight.position);
    float spotLightFactor = dot(rayDirection, spotLight.direction);
    if(spotLightFactor > spotLight.edge) {
        vec4 color = calculatePointLight(spotLight.pointLight);

        return color * (1.0f - (1.0f - spotLightFactor) * (1.0f / (1.0f - spotLight.edge)));
    }
    return vec4(0, 0, 0, 0);
}

vec4 calculateSpotLights() {
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < spotLightCount; i++) {
        totalColor += calculateSpotLight(spotLights[i]);
    }
    return totalColor;
}

vec4 calculatePointLights() {
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < pointLightCount; i++) {
        totalColor += calculatePointLight(pointLights[i]);
    }
    return totalColor;
}

void main() {

    vec4 finalColor = calculateDirectionalLight();
    finalColor += calculatePointLights();
    finalColor += calculateSpotLights();

    color = texture(textureSampler, textureCoordinate) * finalColor;
}
