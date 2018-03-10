#pragma once

#include "Library.h"

#include <stdint.h>
#include <time.h>

class Timer;
extern Timer* GLPT_timer;

class Timer {
public:
	static void Create(void) {GLPT_timer=new Timer;}
	static void Release(void) {delete GLPT_timer;}

	int64_t CreateTicket(unsigned int ms);
	bool TicketCompleted(int64_t tic);
};
