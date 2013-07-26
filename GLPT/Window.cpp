#include "Library.h"
#include "Window.h"

int GLPT_width=640;
int GLPT_height=480;

Window* GLPT_window;

LRESULT CALLBACK wproc(HWND handle,UINT msg,WPARAM wp,LPARAM lp) {

	return GLPT_window->WndProc(handle,msg,wp,lp);

}

void Window::Update(void) {

	MSG m;

	if (PeekMessage(&m,window_handle,0,0,PM_REMOVE)) {
		TranslateMessage(&m);
		DispatchMessage(&m);

		if (m.message==WM_QUIT) {
			req_close=true;
		}
	}

	UpdateWindow(window_handle);
}

EResult Window::Initialize(WindowData wd) {

	EResult output;
	WNDCLASS wc;

	// Similar to Graphics implementation.

	GLPT_width=wd.width;
	GLPT_height=wd.height;

	ZeroMemory(&wc,sizeof wc);

	wc.hInstance=GetModuleHandle(NULL);
	wc.lpfnWndProc=wproc;
	wc.lpszClassName="wclass1";
	wc.style=CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&wc);

	window_handle=CreateWindow("wclass1","GLPT",WS_BORDER,320,240,GLPT_width,GLPT_height,NULL,NULL,GetModuleHandle(NULL),NULL);

	if (!window_handle) {
		output.Error("Failed to create window handle.");
		return output;
	}

	ShowWindow(window_handle,SW_SHOW);
	UpdateWindow(window_handle);
	SetFocus(window_handle);
	SetForegroundWindow(window_handle);

	ShowCursor(false);

	return output;

}

bool Window::RequestedClose(void) {
	return req_close;
}

LRESULT Window::WndProc(HWND handle,UINT msg,WPARAM wp,LPARAM lp) {

	switch(msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		GLPT_input->UpdateCallback(wp,true);
		break;
	case WM_KEYUP:
		GLPT_input->UpdateCallback(wp,false);
		break;
	}


	return DefWindowProc(handle,msg,wp,lp);
}