#version 330 core
#extension GL_ARB_explicit_uniform_location : require

in vec2 UV;
out vec3 color;

uniform sampler2D textureSampler;

void main() {
    color = texture(textureSampler, UV).rgb;
}