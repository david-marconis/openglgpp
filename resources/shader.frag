#version 450 core

in vec2 textureCoordinate;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
	color = texture(textureSampler, textureCoordinate);
}
