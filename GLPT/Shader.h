#ifndef _GLPT_SHADER
#define _GLPT_SHADER

#include "Library.h"
#include "linmath.h"

//  Pre-defined shader semantics.
//  GLPT_texture : currently bound texture.
//  GLPT_blur_intensity : intensity for blurs.
//  GLPT_random_number : random uniform float.
//  GLPT_transform : transform matrix.
//  GLPT_flip_horizontal : flip the texture coordinates horizontally. must be implemented in the pixel shader.
//

class Shader {
public:
	static Shader* GetCurrentShader(void) {
		return Shader::current_shader;
	}

	EResult Initialize(std::string, std::string);

	void Release(void);
	void SetAsCurrent(void) {
		Shader::current_shader=this;
	}

	void Bind(void);
	void Transform(mat4x4);
	void Texture(int);
	void Blur(float);
	void FlipH(bool);
	void SetVariant(std::string,float);

private:
	static Shader* current_shader;
	static Shader* bound_shader;
	int current_texture;
	unsigned int vs, fs, prg, loc_flip, loc_xform;
	std::string shader_name;
};

#endif
