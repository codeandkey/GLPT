#pragma once
#include "GLPT.h"

class Brush : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);
private:
	Drawable<BasicVertex> draw_object;
	PhysBody phys_object;
};