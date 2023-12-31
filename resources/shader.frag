#version 450 core

in vec2 textureCoordinate;
in vec3 Normal;
in vec3 FragPos;
in vec4 DirectionalLightSpacePosition;

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

struct OmniShadowMap {
    samplerCube shadowMap;
    float farPlane;
};

struct Material {
    float specularIntensity;
    float shininess;
};

vec3 gridSamplingDisk[20] = vec3[] (vec3(1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1), vec3(1, 1, -1), vec3(1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1), vec3(1, 1, 0), vec3(1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0), vec3(1, 0, 1), vec3(-1, 0, 1), vec3(1, 0, -1), vec3(-1, 0, -1), vec3(0, 1, 1), vec3(0, -1, 1), vec3(0, -1, -1), vec3(0, 1, -1));

uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D textureSampler;
uniform sampler2D directionalShadowMap;
uniform OmniShadowMap omniShadowMaps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];

uniform Material material;

uniform vec3 eyePosition;

float calculateDirectionalShadowFactor(DirectionalLight light) {
    vec3 projectionCoords = DirectionalLightSpacePosition.xyz / DirectionalLightSpacePosition.w;
    projectionCoords = projectionCoords * 0.5 + 0.5;
    float current = projectionCoords.z;

    vec3 normal = normalize(Normal);
    vec3 ligthDirection = normalize(light.direction);

    float bias = max(0.05 * (1 - dot(normal, ligthDirection)), 0.005);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(directionalShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(directionalShadowMap, projectionCoords.xy + vec2(x, y) * texelSize).r;
            shadow += current - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projectionCoords.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

float calculateOmniShadowFactor(PointLight light, int shadowIndex) {
    vec3 fragToLight = FragPos - light.position;
    float current = length(fragToLight);

    float shadow = 0.0;
    float bias = 0.05;

    int samples = 20;
    float viewDistance = length(eyePosition - FragPos);
    float diskRadius = (1.0 + (viewDistance / omniShadowMaps[shadowIndex].farPlane)) / 25.0;

    // for(int i = 0; i < samples; i++) {
    vec3 position = fragToLight;
    // position += gridSamplingDisk[i] * diskRadius;
    float closest = texture(omniShadowMaps[shadowIndex].shadowMap, position).r;
    closest *= omniShadowMaps[shadowIndex].farPlane;
    if(current - bias > closest) {
        shadow += 1.0;
    }
    // }

    // shadow /= float(samples);
    return shadow;
}

vec4 calculateLightByDirection(Light light, vec3 direction, float shadowFactor) {
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
    return ambientColor + (1.0 - shadowFactor) * (diffuseColor + specularColor);
}

vec4 calculateDirectionalLight() {
    float shadowFactor = calculateDirectionalShadowFactor(directionalLight);
    return calculateLightByDirection(directionalLight.base, directionalLight.direction, shadowFactor);
}

vec4 calculatePointLight(PointLight pointLight, int shadowIndex) {
    vec3 direction = FragPos - pointLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    float shadowFactor = calculateOmniShadowFactor(pointLight, shadowIndex);

    vec4 color = calculateLightByDirection(pointLight.base, direction, shadowFactor);
    float attenuation = pointLight.quadratic * distance * distance +
        pointLight.linear * distance +
        pointLight.constant;
    return color / attenuation;
}

vec4 calculateSpotLight(SpotLight spotLight, int shadowIndex) {
    vec3 rayDirection = normalize(FragPos - spotLight.pointLight.position);
    float spotLightFactor = dot(rayDirection, spotLight.direction);
    if(spotLightFactor > spotLight.edge) {
        vec4 color = calculatePointLight(spotLight.pointLight, shadowIndex);

        return color * (1.0f - (1.0f - spotLightFactor) * (1.0f / (1.0f - spotLight.edge)));
    }
    return vec4(0, 0, 0, 0);
}

vec4 calculateSpotLights() {
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < spotLightCount; i++) {
        totalColor += calculateSpotLight(spotLights[i], i + pointLightCount);
    }
    return totalColor;
}

vec4 calculatePointLights() {
    vec4 totalColor = vec4(0, 0, 0, 0);
    for(int i = 0; i < pointLightCount; i++) {
        totalColor += calculatePointLight(pointLights[i], i);
    }
    return totalColor;
}

void main() {

    vec4 finalColor = calculateDirectionalLight();
    finalColor += calculatePointLights();
    finalColor += calculateSpotLights();

    color = texture(textureSampler, textureCoordinate) * finalColor;
}
