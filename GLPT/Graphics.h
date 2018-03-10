#ifndef _GLPT_GRAPHICS
#define _GLPT_GRAPHICS

#include "Library.h"
#include "Drawable.h"

#include <GLFW/glfw3.h>

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
	EResult Initialize(WindowData);
	GLFWwindow* GetGraphicsDevice(void) {return win;}
	void BindToScreenTarget(void);
	void UnbindFromScreenTarget(void);
	void Clear(void);
	void Present(void);
private:
	Graphics(void) {}
	~Graphics(void);
	Shader* shader_post_process;
	WindowData window_data;
	Drawable<BasicVertex> render_drawable;

	GLFWwindow* win;
};

#endif
