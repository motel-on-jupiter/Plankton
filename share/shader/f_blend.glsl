#version 140

#define lowp
#define mediump
#define highp

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float blend0;
uniform float blend1;

void main(void) {
  gl_FragColor =
    texture2DProj(texture0, gl_TexCoord[0]) * blend0 +
    texture2DProj(texture1, gl_TexCoord[1]) * blend1;
}