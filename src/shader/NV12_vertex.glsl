#version 330 core
#extension GL_ARB_explicit_uniform_location : require

in vec4 vertexIn;
in vec2 textureIn;
out vec2 textureOut;

void main() {
    gl_Position = vertexIn;
    textureOut = textureIn;
    
}