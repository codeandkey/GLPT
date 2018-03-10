#include "Library.h"
#include "Shader.h"
#include "Graphics.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GLXW/glxw.h>

struct shd_group {
	unsigned int prg, vs, fs, loc_xform, loc_flip, loc_texture;
};

/* super quick hack */
static int _t2_init_shdobj(unsigned* out, const char* str, int len, int type);
static int t2_shd_init_filename(shd_group* p, const char* vsf, const char* fsf);
static int t2_shd_init_str(shd_group* p, const char* vs, const char* fs);

Shader* Shader::current_shader=NULL;
Shader* Shader::bound_shader=NULL;

EResult Shader::Initialize(std::string vs_filename, std::string fs_filename) {
	EResult er;

	shd_group s;

	/* copied from the phresh t2 sources */
	if (t2_shd_init_filename(&s, vs_filename.c_str(), fs_filename.c_str())) {
		er.Error("Error loading shaders");
		return er;
	}

	prg = s.prg;
	vs = s.vs;
	fs = s.fs;

	loc_xform = s.loc_xform;
	loc_flip = s.loc_flip;

	glUniform1i(s.loc_texture, 0);
	glActiveTexture(GL_TEXTURE0);

	return er;
}

void Shader::Bind(void) {
	glUseProgram(prg);
	Shader::bound_shader=this;
}

void Shader::Release(void) {
	glDetachShader(prg, vs);
	glDetachShader(prg, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteProgram(prg);
}

void Shader::Texture(int texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Shader::Transform(mat4x4 transform) {
	glUniformMatrix4fv(loc_xform, 1, 0, (float*) *transform);
}

void Shader::Blur(float blur) {
}

void Shader::FlipH(bool flip) {
	glUniform1i(loc_flip, flip ? 1 : 0);
}

void Shader::SetVariant(std::string name,float value) {
}

static char* _t2_fstr(const char* fn) {
	int fs;
	char* out;
	FILE* f = fopen(fn, "r");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	fs = ftell(f);
	fseek(f, 0, SEEK_SET);
	out = (char*) malloc(fs + 1);
	if (!out) {
		fclose(f);
		return NULL;
	}
	out[fs] = 0;
	fread(out, 1, fs, f);
	fclose(f);
	return out;
}

int _t2_init_shdobj(unsigned* out, const char* str, int len, int type) {
	*out = glCreateShader(type);
	int status;
	glShaderSource(*out, 1, &str, &len);
	glCompileShader(*out);
	glGetShaderiv(*out, GL_COMPILE_STATUS, &status);
	if (!status) {
		char logbuf[1024];
		glGetShaderInfoLog(*out, 1024, NULL, logbuf);
		printf("shader compile error: %s\n", logbuf);
		return 1;
	}
	return 0;
}

int t2_shd_init_filename(shd_group* p, const char* vs, const char* fs) {
	char* vsd = _t2_fstr(vs), *fsd = _t2_fstr(fs);
	if (!vsd) {
		printf("failed to open VS %s for reading\n", vs);
		return 1;
	}
	if (!fsd) {
		printf("failed to open FS %s for reading\n", fs);
		return 1;
	}
	int res = t2_shd_init_str(p, vsd, fsd);
	free(vsd);
	free(fsd);
	return res;
}

int t2_shd_init_str(shd_group* p, const char* vs, const char* fs) {
	int res = 0;
	res |= _t2_init_shdobj(&p->vs, vs, strlen(vs), GL_VERTEX_SHADER);

	if (res) {
		printf("Failed to initalize VS shader object\n");
		return 1;
	}

	res |= _t2_init_shdobj(&p->fs, fs, strlen(fs), GL_FRAGMENT_SHADER);

	if (res) {
		printf("Failed to initalize FS shader object\n");
		return 1;
	}

	p->prg = glCreateProgram();
	glAttachShader(p->prg, p->vs);
	glAttachShader(p->prg, p->fs);
	glLinkProgram(p->prg);

	glGetProgramiv(p->prg, GL_LINK_STATUS, &res);
	if (!res) {
		char logbuf[1024];
		glGetProgramInfoLog(p->prg, 1024, NULL, logbuf);
		printf("Shader program link fail: %s\n", logbuf);
		return 1;
	}

	glUseProgram(p->prg);

	p->loc_xform = glGetUniformLocation(p->prg, "GLPT_transform");
	p->loc_texture = glGetUniformLocation(p->prg, "GLPT_texture");
	p->loc_flip = glGetUniformLocation(p->prg, "GLPT_flip");

	glUniform1i(p->loc_texture, 0);

	return 0;
}
