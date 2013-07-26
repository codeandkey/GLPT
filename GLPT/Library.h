#ifndef _LIB_H
#define _LIB_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>

#include <Windows.h>

#include <Windowsx.h>
#include <d3d10.h>
#include <D3DX10.h>
#include <DXGI.h>

#pragma comment (lib,"d3d10.lib")
#pragma comment (lib,"d3dx10.lib")
#pragma comment (lib,"dxgi.lib")

#include <Box2D/Box2D.h>

#pragma comment (lib,"Box2D.lib")

class Logger {
public:
	void Print(std::string log) {
		log_vector.push_back(log);
		log_stream << log << "\n";
		std::cout << log << "\n";
	}
	void WriteToFile(std::string filename) {
		std::ofstream file(filename);
		while(!log_stream.eof()) {
			file.put(log_stream.get());
		}
		file.close();
	}

	std::vector<std::string> log_vector;
	std::stringstream log_stream;
};

static Logger GLPT_logger;

struct Layout {
	std::string name;
	DXGI_FORMAT type;
};

struct BasicVertex {
	float x,y,z;
	float tx,ty;
	static Layout layout[2];
	static unsigned int layout_size;
	static void make_rectangle(BasicVertex* vertices_6,float w,float h);
};

struct EResult {
	bool Error(void) {
		return err;
	}
	void Error(std::string m) {
		err=true;
		msg=m;
	}
	std::string ErrorMessage(void) {
		return msg;
	}
	EResult(void) : err(false), msg("") {}
private:
	bool err;
	std::string msg;
};

struct EntityData {
	std::string type;
	std::string identity;
	std::map<std::string,std::string> data_map;
};

struct WindowData {
	int width,height;
	bool fullscreen,vertical_sync;
	void Native(void) {
		RECT win_rect;
		GetWindowRect(GetDesktopWindow(),&win_rect);
		width=win_rect.right;
		height=win_rect.bottom;
	}
};

extern int GLPT_width;
extern int GLPT_height;

#endif