#pragma once

#include "GLPT.h"

class Parallax : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);
private:
	std::vector<Drawable<BasicVertex>> parallax_buffer;
};