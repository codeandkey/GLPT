#pragma once
#include "GLPT.h"

class Block : public Entity {
public:
	void EventDraw(EntityLoadData*);
	void EventStep(void);
	void EventDraw(void);
private:
	Drawable<BasicVertex> draw_object;
	PhysBody phys_object;
};