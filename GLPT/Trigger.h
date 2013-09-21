#pragma once

#include "GLPT.h"

class Trigger : public Entity {
public:
	void EventCreate(EntityLoadData*);
	void EventDraw(void);
	void EventStep(void);

	virtual void TriggerCreate(void) {}
	virtual void TriggerDraw(void) {}
	virtual void TriggerStep(void) {}

	virtual void TriggerOnPlayerTouch(void) {}
	virtual void TriggerOnPlayerDie(void) {}
	virtual void TriggerOnPlayerMove(void) {}
private:
	std::string block_type;
};