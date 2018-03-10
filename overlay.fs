#version 120

uniform sampler2D GLPT_texture;
varying vec2 out_texcoord;

void main(void) {
	gl_FragColor = texture2D(GLPT_texture, out_texcoord);
	gl_FragColor = vec4(1, 1, 1, 1);
}
