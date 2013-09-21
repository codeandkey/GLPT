#pragma once

#include "GLPT.h"

class Player : public Entity {
public:
	void EventCreate(EntityLoadData* data);
	void EventStep(void);
	void EventDraw(void);
	void EventDestroy(void);
	void GetPosition(float* x=NULL,float* y=NULL);

	bool Grounded(void);
private:
	b2Vec2 initial_position;
	bool initial_player_spawn;
	Animation* ani;
	Drawable<BasicVertex> draw_object;
	PhysBody phys_object;
};