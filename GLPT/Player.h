#pragma once

#include "GLPT.h"

class Player : public Entity {
public:
	void EventCreate(EntityLoadData* data);
	void EventStep(void);
	void EventDraw(void);
	void GetPosition(float* x=NULL,float* y=NULL);
private:
	Animation* ani;
	Drawable<BasicVertex> draw_object;
	PhysBody phys_object;
};