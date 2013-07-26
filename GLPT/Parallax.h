#pragma once

#include "GLPT.h"

class Parallax : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);
private:
	Drawable<BasicVertex> front_object,back_object;
};