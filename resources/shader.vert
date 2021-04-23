#version 450 core
layout(location = 0) in vec3 pos;
out vec4 vertexColor;
uniform mat4 model;
uniform mat4 projection;
void main(void) {
    gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);
    vertexColor = vec4(clamp(pos, 0.0, 1.0), 1.0);
}
