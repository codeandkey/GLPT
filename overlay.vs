#version 120

uniform int GLPT_flip;
uniform mat4x4 GLPT_transform;

attribute vec3 pos;
attribute vec2 texcoord;

varying vec2 out_texcoord;

void main(void) {
	gl_Position = GLPT_transform * vec4(pos, 1.0);
	out_texcoord = texcoord;

	if (GLPT_flip != 0) { 
		out_texcoord.x = 1.0f - texcoord.x;
	}
}
