#version 330 core
#extension GL_ARB_explicit_uniform_location : require

out vec2 textureOut;

uniform sampler2D sTextureY;
uniform sampler2D sTextureU;
uniform sampler2D sTextureV;

void main() {
    vec3 yuv;
    vec3 rgb;

    yuv.x = texture2D(sTextureY, textureOut).r;
    yuv.y = texture2D(sTextureU, textureOut).r - 0.5;
    yuv.z = texture2D(sTextureV, textureOut).r - 0.5;

    rgb = mat3(1,1,1, 0,-0.39465,2.03211, 1.13983,-0.58060,0)*yuv;
    gl_FragColor = vec4(rgb,1);









    

}