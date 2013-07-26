#ifndef _GLPT_WINDOW
#define _GLPT_WINDOW

#include "Library.h"
#include "Input.h"

class Window;

extern Window* GLPT_window;

class Window {
public:
	Window(void) {GLPT_window=this;req_close=false;}
	EResult Initialize(WindowData wd);
	bool RequestedClose(void);
	LRESULT WndProc(HWND handle,UINT msg,WPARAM wp,LPARAM lp);
	void Update(void);
	HWND WindowHandle(void) {return window_handle;}
private:
	HWND window_handle;
	bool req_close;
};


#endif