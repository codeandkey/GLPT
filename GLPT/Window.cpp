#include "Library.h"
#include "Window.h"
#include "Graphics.h"

#include <GLFW/glfw3.h>

int GLPT_width=640;
int GLPT_height=480;

Window* GLPT_window;

extern Graphics* GLPT_graphics;

void Window::Update(void) {
	glfwPollEvents();
}

EResult Window::Initialize(WindowData wd) {
	EResult output;
	return output;
}

bool Window::RequestedClose(void) {
	if (GLPT_graphics) {
		return glfwWindowShouldClose(GLPT_graphics->GetGraphicsDevice());
	}

	return false; /* TODO : use glfwWindowShouldClose */
}
