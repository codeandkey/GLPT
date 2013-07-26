#ifndef _GLPT_GRAPHICS
#define _GLPT_GRAPHICS

#include "Library.h"
#include "Drawable.h"

class Graphics;

extern Graphics* GLPT_graphics;

class Graphics {
public:
	static void Create(void) {
		GLPT_graphics=new Graphics;
	}
	static void Release(void) {
		delete GLPT_graphics;
	}
	EResult Initialize(WindowData,std::string);
	ID3D10Device* GetGraphicsDevice(void) {return dx_device;}
	void Clear(void);
	void Present(void);
private:
	Graphics(void) {}
	~Graphics(void);
	Shader* shader_post_process;
	WindowData window_data;
	Drawable<BasicVertex> render_drawable;

	ID3D10Device* dx_device;
	IDXGISwapChain* dx_swap_chain;
	ID3D10RenderTargetView* dx_screen_target,*dx_texture_target;
	ID3D10Texture2D* dx_depth_texture,*dx_render_texture;
	ID3D10ShaderResourceView* dx_render_texture_resource;
	ID3D10DepthStencilView* dx_depth_stencil;
};

#endif