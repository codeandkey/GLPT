#include "Library.h"
#include "Input.h"

#include <GLFW/glfw3.h>

std::map<ControlKey,int> Input::control_map;

Input* GLPT_input;

void Input::UpdateCallback(int key,bool press) {
	key_states[key]=press;
}

bool Input::KD(int key) {
	return key_states[key];
}

bool Input::KD(ControlKey key) {
	try {
		control_map.at(key);
	} catch(std::out_of_range) {
		return false;
	}

	return key_states[control_map[key]];
}

void Input::DefaultControls(void) {
	control_map.clear();
	control_map[C_MOVE_RIGHT]=GLFW_KEY_RIGHT;
	control_map[C_MOVE_LEFT]=GLFW_KEY_LEFT;
	control_map[C_JUMP]=GLFW_KEY_SPACE;
}
