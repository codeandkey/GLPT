#pragma once
#include "Library.h"
#include <chrono>

class Animation {
public:
	void CreateFromFile(std::string filename);
	ID3D10ShaderResourceView* GetCurrentTexture(void);
	void UpdateAnimation(void);
	void SetAnimationState(std::string event_state);
private:
	std::string current_event;
	bool change_animation;
	std::map<std::string,std::vector<ID3D10ShaderResourceView*>> animation_buffer;
	std::map<std::string,std::vector<unsigned int>> animation_timer_buffer;

	unsigned int current_animation_index;
	unsigned int ticket;
};