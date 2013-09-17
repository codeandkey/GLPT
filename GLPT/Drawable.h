#ifndef _GLPT_DRAWABLE
#define _GLPT_DRAWABLE

#include "Library.h"
#include "Indexer.h"
#include "Shader.h"
#include "Camera.h"
#include "Animation.h"
#include "Timer.h"

template <typename T> class Drawable {
public:
	Drawable(void) {
		draw_mode=D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		texture_animation=NULL;
		hflip=false;
	}
	void SetAnimation(Animation* a) {
		texture_animation=a;
	}
	void Load(T* vertex_data,unsigned int vertex_count,Shader* render_shader=Shader::GetCurrentShader()) {
		D3D10_BUFFER_DESC dx_buffer_desc;
		D3D10_SUBRESOURCE_DATA dx_init_data;
		HRESULT hr;
		this->render_shader=render_shader;
		this->vertex_count=vertex_count;

		ZeroMemory(&dx_buffer_desc,sizeof dx_buffer_desc);

		dx_buffer_desc.BindFlags=D3D10_BIND_VERTEX_BUFFER;
		dx_buffer_desc.ByteWidth=sizeof(T)*vertex_count;
		dx_buffer_desc.CPUAccessFlags=0;
		dx_buffer_desc.Usage=D3D10_USAGE_DEFAULT;
		dx_buffer_desc.MiscFlags=0;

		ZeroMemory(&dx_init_data,sizeof dx_init_data);

		dx_init_data.pSysMem=vertex_data;
		dx_init_data.SysMemPitch=sizeof(T)*vertex_count;
		dx_init_data.SysMemSlicePitch=0;

		hr=GLPT_graphics->GetGraphicsDevice()->CreateBuffer(&dx_buffer_desc,&dx_init_data,&dx_vertex_buffer);
	}
	void Texturize(std::string filename) {
		texture=Indexer::LoadTexture(filename);
	}
	void Texturize(ID3D10ShaderResourceView* tex) {
		texture=tex;
	}
	void Flip(bool horizontal) {
		hflip=horizontal;
	}
	void Release(void) {
		glDeleteBuffers(1,&vertex_buffer);
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

		render_shader->Transform(GLPT_camera->GetTransform());

		render_shader->Update();
		render_shader->Texture(texture);
		render_shader->FlipH(hflip);

		render_shader->Bind();

		GLPT_graphics->GetGraphicsDevice()->IASetPrimitiveTopology(draw_mode);
		GLPT_graphics->GetGraphicsDevice()->IASetVertexBuffers(0,1,&dx_vertex_buffer,&stride,&offset);

		GLPT_graphics->GetGraphicsDevice()->Draw(vertex_count,0);

	}
private:
	bool hflip;
	Animation* texture_animation;
	Shader* render_shader;
	ID3D10ShaderResourceView* texture;
	ID3D10Buffer* dx_vertex_buffer;
	unsigned int vertex_count;
	D3D10_PRIMITIVE_TOPOLOGY draw_mode;
};

#endif