#ifndef _GLPT_ENGINE
#define _GLPT_ENGINE

#include "Window.h"
#include "Shader.h"
#include "Graphics.h"

class Engine;

extern Engine* GLPT_engine;

class Engine {
public:
	static void Create(void) {
		GLPT_engine=new Engine;
	}
	EResult Initialize(int,char**,void(*)(void)=NULL);
	void End(void);
	void Begin(void);
	~Engine(void);
	Engine(void);
private:
	Window* window_handle;

	bool requested_terminate;
};



#endif