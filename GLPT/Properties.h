#pragma once

#include "GLPT.h"

class Property : public Entity {
public:
	void EventCreate(EntityLoadData* data);
	float GetDeathPosition(void);
private:
	float death_position;
};