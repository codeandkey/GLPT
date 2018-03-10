#pragma once
#include "Library.h"

class Animation {
public:
	void CreateFromFile(std::string filename);
	int GetCurrentTexture(void);
	void UpdateAnimation(void);
	void SetAnimationState(std::string event_state);
	std::string GetAnimationState(void) { return current_event; }
	Animation(void);
private:
	std::string current_event;
	bool change_animation;
	std::map<std::string,std::vector<int>> animation_buffer;
	std::map<std::string,std::vector<unsigned int>> animation_timer_buffer;
	unsigned int current_animation_index;
	int64_t ticket;
};
