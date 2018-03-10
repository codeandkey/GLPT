#pragma once

#include "GLPT.h"

#define MAX_PARALLAX_LAYERS 32

struct ParallaxObject {
	float x_offset,y_offset;

	Drawable<BasicVertex>* drawable_handle;
	ParallaxObject(void) {
		x_offset=y_offset=0.0f;
		drawable_handle=NULL;
	}
};

class Parallax : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);
private:
	std::vector<ParallaxObject> parallax_buffer[MAX_PARALLAX_LAYERS];
};
