#version 120

uniform sampler2D GLPT_texture;
varying vec2 out_texcoord;

void main(void) {
	vec4 pre = texture2D(GLPT_texture, out_texcoord);
	if (pre.a == 0.0f) discard;
	gl_FragColor = pre;
}
