#version 330 core
#extension GL_ARB_explicit_uniform_location : require

in vec2 textureIn;
out vec2 textureOut;

layout(location = 0) in vec3 aPos;
layout(location = 2) uniform mat4 model;
layout(location = 3) uniform mat4 view;
layout(location = 4) uniform mat4 proj;
layout(location = 5) in vec2 vertexUV;

out vec2 UV;

void main() {
    gl_Position = proj * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    UV = vertexUV;
    textureOut = textureIn;
}