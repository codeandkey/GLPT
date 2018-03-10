#include "Library.h"
#include "Graphics.h"
#include "Drawable.h"
#include "Window.h"
#include "Input.h"

Graphics* GLPT_graphics;

void input_callback(GLFWwindow* win, int key, int scancode, int state, int mods);

EResult Graphics::Initialize(WindowData wd) {
	EResult output, good;
	this->window_data=wd;

	output.Error("Failed to init graphics");

	if (!glfwInit()) return output;
	if (!(win = glfwCreateWindow(wd.width, wd.height, "GLPT", wd.fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL))) return output;

	glfwMakeContextCurrent(win);
	glfwSetKeyCallback(win, input_callback);

	if (glxwInit()) return output;

	GLPT_logger.Print("[GLPT_graphics] Initialized graphics.");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	//glDisable(GL_DEPTH_TEST);
	//glDisable(GL_CULL_FACE);

	GLPT_width = wd.width;
	GLPT_height = wd.height;

	return good;
}

void Graphics::Clear(void) {
	glClearColor(0.0f, 0.2f, 0.27f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Graphics::Present(void) {
	glfwSwapBuffers(win);
}

Graphics::~Graphics(void) {
	glfwDestroyWindow(win);
	glfwTerminate();
}

void input_callback(GLFWwindow* win, int key, int scan, int state, int mods) {
	Input* ih = (Input*) GLPT_input;

	ih->UpdateCallback(key, state == GLFW_PRESS);
}
