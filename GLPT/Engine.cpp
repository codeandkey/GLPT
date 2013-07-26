#include "Library.h"
#include "Window.h"
#include "Engine.h"
#include "Input.h"
#include "Graphics.h"
#include "Iterator.h"
#include "Physics.h"
#include "Camera.h"
#include "Timer.h"

// GLOBAL static defines here.

	// BasicVertex static members :
		Layout BasicVertex::layout[2] = {
			"position",DXGI_FORMAT_R32G32B32_FLOAT,
			"texcoord",DXGI_FORMAT_R32G32_FLOAT
		};
		unsigned int BasicVertex::layout_size=2;
		void BasicVertex::make_rectangle(BasicVertex* vertices,float w,float h) {

			vertices[0].x=-w; // Tri1 : Bottom left.
			vertices[0].y=h;
			vertices[0].z=0;
			vertices[0].tx=0;
			vertices[0].ty=0;

			vertices[1].x=w;
			vertices[1].y=-h;
			vertices[1].z=0;
			vertices[1].tx=w;
			vertices[1].ty=h;

			vertices[2].x=-w;
			vertices[2].y=-h;
			vertices[2].z=0;
			vertices[2].tx=0;
			vertices[2].ty=h;

			vertices[3].x=-w;
			vertices[3].y=h;
			vertices[3].z=0;
			vertices[3].tx=0;
			vertices[3].ty=0;

			vertices[4].x=w;
			vertices[4].y=h;
			vertices[4].z=0;
			vertices[4].tx=w;
			vertices[4].ty=0;

			vertices[5].x=w;
			vertices[5].y=-h;
			vertices[5].z=0;
			vertices[5].tx=w;
			vertices[5].ty=h;
			
		}
	// end

// End global static defines.

Engine* GLPT_engine;

static std::string GetArgumentValue(int argc,char** argv,std::string target) {

	for(int i=0;i<argc-1;i++) {
		if (argv[i]==target) {
			return argv[i+1];
		}
	}

	return "";

}

static bool ArgumentSet(int argc,char** argv,std::string target) {

	for(int i=0;i<argc;i++) {
		if (argv[i]==target) {
			return true;
		}
	}

	return false;

}

EResult Engine::Initialize(int argc,char** argv,void(*bind_ents)(void)) {

	EResult output;

	Input::Create();
	Iterator::Create();
	Physics::Create();
	Camera::Create();
	Graphics::Create();
	Timer::Create();

	WindowData win_init_data;
	std::string init_map;

	win_init_data.fullscreen=ArgumentSet(argc,argv,"-fullscreen");
	win_init_data.vertical_sync=ArgumentSet(argc,argv,"-vertical_sync");
	win_init_data.width=atoi(GetArgumentValue(argc,argv,"-width").c_str());
	win_init_data.height=atoi(GetArgumentValue(argc,argv,"-height").c_str());
	init_map=GetArgumentValue(argc,argv,"-map");
	if (init_map=="") init_map="default";
	if (ArgumentSet(argc,argv,"-native_resolution")) win_init_data.Native();

	if (!win_init_data.width) win_init_data.width=640;
	if (!win_init_data.height) win_init_data.height=480;

	window_handle=new Window();

	output=window_handle->Initialize(win_init_data);

	if (output.Error()) {
		return output;
	}

	output=GLPT_graphics->Initialize(win_init_data,"post_process");

	if (output.Error()) {
		return output;
	}

	if (bind_ents) bind_ents();

	GLPT_iterator->Load(init_map,false);

	GLPT_logger.Print("[GLPT_engine] Initialized engine.");

	return output;
}

Engine::~Engine(void) {

	if (window_handle) {
		delete window_handle;
		window_handle=NULL;
	}

	Input::Release();
	Iterator::Release();
	Physics::Release();
	Camera::Release();
	Graphics::Release();
	Timer::Release();

}

Engine::Engine(void) {
	GLPT_engine=this;
	window_handle=NULL;
}

void Engine::Begin(void) {
	requested_terminate=false;

	while(!requested_terminate) {

		GLPT_window->Update();

		GLPT_physics->UpdateWorld();
		GLPT_iterator->BroadcastStep();
		GLPT_graphics->Clear();
		GLPT_iterator->BroadcastDraw();

		GLPT_graphics->Present();

		if (GLPT_input->KD(27)) End();
		if (window_handle->RequestedClose()) End();

		//Sleep(1000/60); // Manual FPS limit, for now.
	}
}

void Engine::End(void) {
	requested_terminate=true;
}