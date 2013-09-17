#pragma once

#include "GLPT.h"

class GameCamera : public Entity {
public:

	void EventCreate(EntityLoadData*);
	void FollowPlayer(bool);
	void EventStep(void);

	void GetPosition(float* x,float* y,float* z);

private:

	float x,y,z,tx,ty,tz;

	bool following_player;

};
