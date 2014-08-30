#version 120

#define lowp
#define mediump
#define highp

uniform sampler2D texture;

void main(void) {
  gl_FragColor = texture2DProj(texture, gl_TexCoord[0]);
}