#pragma once

#include "GLPT.h"

class GameCamera : public Entity {
public:

	void EventCreate(EntityLoadData*);
	void FollowPlayer(bool);
	void EventStep(void);

private:

	float x,y,z,tx,ty,tz;

	bool following_player;

};
