#version 330 core
#extension GL_ARB_explicit_uniform_location : require

/** A fragment shader to convert NV12 to RGB.
  * Input textures Y - is a block of size w*h. 
  * texture UV is of size w*h/2. 
  * Remember, both U and V are individually of size w/2*h/2, but they are interleaved.
  * The layout looks like this :
  * ----------
  * |        |
  * |   Y    | size = w*h
  * |        |
  * |________|
  * |UVUVUVUV|size = w/2*h/2
  * |UVUVUVUV|size = w/2*h/2
  * ----------
  */

layout(location = 10) out vec4 fragColor;

precision highp float;
varying vec2 vTextureCoord;
uniform sampler2D sTextureY;
uniform sampler2D sTextureUV;
uniform float sBrightnessValue;
uniform float sContrastValue;

void main() {
    float r,g,b,y,u,v;

    // We had put the Y values of each pixel to the R,G,B components by GL_LUMINANCE,
    // that's why we're pulling it from the R component, we could also use G or B

    y = texture2D(sTextureY, vTextureCoord).r;

    // We had put the U and V values of each pixel to the A and R,G,B components of the
    // texture respectively using GL_LUMINANCE_ALPHA. Since U,V bytes are interspread
    // in the texture, this is probably the fastest way to use them in the shader

    u = texture2D(sTextureUV, vTextureCoord).r - 0.5;
    v = texture2D(sTextureUV, vTextureCoord).a - 0.5;

    // The numbers are just YUV to RGB conversion constants

    r = y + 1.13983*v;
    g = y - 0.39465*u - 0.58060*v;
    b = y + 2.03211*u;
    r = r * sContrastValue + sBrightnessValue;
    g = g * sContrastValue + sBrightnessValue;
    b = b * sContrastValue + sBrightnessValue;

    // We finally set the RGB color of our pixel
    fragColor = vec4(r, g, b, 1.0);  
}