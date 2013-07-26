#pragma once

#include "GLPT.h"

enum RenderState {
	RENDER_STATE_NORMAL
};

class RenderController : public Entity {
public:
	void EventCreate(EntityLoadData* data);
	void EventStep(void);
private:
	Shader* render_normal;
	RenderState current_render_state;
	std::map<RenderState,Shader*> render_index;
};