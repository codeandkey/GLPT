#ifndef _LIB_H
#define _LIB_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <ctime>

#include <Box2D/Box2D.h>

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

struct BasicVertex {
	float x,y,z;
	float tx,ty;
	static void make_rectangle(BasicVertex* vertices_6,float w,float h,float stretch_width=0.0f,float stretch_height=0.0f);
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
		/* not implemented */
	}
};

extern int GLPT_width;
extern int GLPT_height;

#endif
