#version 330 core
#extension GL_ARB_explicit_uniform_location : require

out vec2 textureOut;
out vec3 color;

uniform sampler2D sTextureY;
uniform sampler2D sTextureUV;

void main() {
    vec3 yuv;
    vec3 rgb;

    yuv.x = texture2D(sTextureY, textureOut).r;
    yuv.y = texture2D(sTextureUV, textureOut).r - 0.5;
    yuv.z = texture2D(sTextureUV, textureOut).a - 0.5;

    rgb = mat3(1,1,1, 0,-0.39465,2.03211, 1.13983,-0.58060,0)*yuv;
    color = rgb;
}


