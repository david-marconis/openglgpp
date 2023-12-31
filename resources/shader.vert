#version 450 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texture;
layout(location = 2) in vec3 normal;

out vec2 textureCoordinate;
out vec3 Normal;
out vec3 FragPos;
out vec4 DirectionalLightSpacePosition;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 directionalLightTransform;

void main() {
    gl_Position = projection * view * model * vec4(pos, 1.0);
    DirectionalLightSpacePosition = directionalLightTransform * model * vec4(pos, 1.0);
    textureCoordinate = texture;
    // transpose inverse to account for non uniform scaling
    Normal = mat3(transpose(inverse(model))) * normal;
    FragPos = (model * vec4(pos, 1.0)).xyz;
}
