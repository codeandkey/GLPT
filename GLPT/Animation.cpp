#include "Animation.h"
#include "Indexer.h"
#include "Timer.h"

Animation::Animation(void) {
	current_event="";
	current_animation_index=0;
}

void Animation::SetAnimationState(std::string state) {
	if (current_event==state) return;
	try {
		animation_buffer.at(state);
	} catch(std::out_of_range) {
		GLPT_logger.Print("[GLPT_animation] No state " + state + " found!");
		return;
	}

	current_event=state;
	current_animation_index=0;

	if (!animation_buffer[current_event].size()) return;
	if (!animation_timer_buffer[current_event][0]) return;

	ticket=GLPT_timer->CreateTicket(animation_timer_buffer[current_event][0]);
}

void Animation::CreateFromFile(std::string filename) {
	
	filename="Resource/Texture/"+filename;

	std::ifstream file(filename);

	if (!file) {
		GLPT_logger.Print("[GLPT_animation] Animation file " + filename + " not found!");
		return;
	}

	std::string file_input;

	while(!file.eof()) {
		file >> file_input;
		if (file_input[0]=='#') {
			std::getline(file,file_input);
			continue;
		}

		if (file_input=="animation") {
			std::string event_name="",texture_name="";
			unsigned int duration=0;

			file >> file_input;

			event_name=file_input;
			current_event=event_name;

			while(true) {

				file >> texture_name;

				if (texture_name=="end") break;

				file >> duration;
				animation_buffer[event_name].push_back(Indexer::LoadTexture(texture_name));
				animation_timer_buffer[event_name].push_back(duration);
			}
		}
	}

	ticket=GLPT_timer->CreateTicket(animation_timer_buffer[current_event][0]);
}

void Animation::UpdateAnimation(void) {

	try {
		animation_buffer.at(current_event);
	} catch(std::out_of_range) {
		return;
	}

	static unsigned int next_anim_index=0;

	if (GLPT_timer->TicketCompleted(ticket)) {
		next_anim_index++;
		if (next_anim_index>=animation_buffer[current_event].size()) next_anim_index=0;
		current_animation_index=next_anim_index;
		
		if (!animation_timer_buffer[current_event][current_animation_index]) return;

		ticket=GLPT_timer->CreateTicket(animation_timer_buffer[current_event][current_animation_index]);
	}

}

ID3D10ShaderResourceView* Animation::GetCurrentTexture(void) {
	try {
		animation_buffer.at(current_event).at(current_animation_index);
	} catch(std::out_of_range) {
		GLPT_logger.Print("[GLPT_animation] Failed to index " + current_event);
		return NULL;
	}

	return animation_buffer[current_event][current_animation_index];
}