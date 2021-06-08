#version 450 core

in vec4 FragPos;

uniform vec3 lightPosition;
uniform float farPlane;

void main() {
    float distance = length(FragPos.xyz - lightPosition);
    distance /= farPlane;
    gl_FragDepth = distance;
}
