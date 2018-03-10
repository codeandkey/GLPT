#ifndef _GLPT_DRAWABLE
#define _GLPT_DRAWABLE

#include "Library.h"
#include "Indexer.h"
#include "Shader.h"
#include "Camera.h"
#include "Animation.h"
#include "Timer.h"

#include <GLXW/glxw.h>

template <typename T> class Drawable {
public:
	Drawable(void) {
		texture_animation=NULL;
		hflip=false;
	}
	void SetAnimation(Animation* a) {
		texture_animation=a;
	}
	void Load(T* vertex_data,unsigned int vertex_count,Shader* render_shader=Shader::GetCurrentShader()) {
		this->render_shader=render_shader;
		this->vertex_count=vertex_count;

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(T), vertex_data, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), NULL);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
	}
	void Texturize(std::string filename) {
		texture=Indexer::LoadTexture(filename);
	}
	void Texturize(int tex) {
		texture=tex;
	}
	void Flip(bool horizontal) {
		hflip=horizontal;
	}
	void Release(void) {
		glDeleteVertexArrays(1, &vao);
		glDeleteBuffers(1, &vbo);
	}
	void Draw(float x=0.0f,float y=0.0f,float angle_r=0.0f,float z=0.0f) {
		unsigned int stride=sizeof(T),offset=0;

		if (texture_animation) {
			texture_animation->UpdateAnimation();
			texture=texture_animation->GetCurrentTexture();
		}

		if (!render_shader) {
			render_shader=Shader::GetCurrentShader();
			if (!render_shader) {
				GLPT_logger.Print("[GLPT_drawable] Drawable object drawing without shader!");
				return;
			}
		}

		GLPT_camera->DrawAngle(angle_r,0,0);
		GLPT_camera->DrawPosition(x,y,z);

		render_shader->Bind();

		mat4x4 xf;
		GLPT_camera->GetTransform(xf);
		render_shader->Transform(xf);

		render_shader->Texture(texture);
		render_shader->FlipH(hflip);

		glBindVertexArray(vao);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, vertex_count);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}
private:
	bool hflip;
	Animation* texture_animation;
	Shader* render_shader;
	unsigned int vertex_count;
	unsigned int vbo, vao, texture;
};

#endif
