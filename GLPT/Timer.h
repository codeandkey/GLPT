#pragma once

#include "Library.h"

#include <stdint.h>
#include <time.h>

class Timer;
extern Timer* GLPT_timer;

struct Ticket {
	unsigned int ticket_id;
	uint64_t duration, start_time;
};

class Timer {
public:
	static void Create(void) {GLPT_timer=new Timer;}
	static void Release(void) {delete GLPT_timer;}

	unsigned int CreateTicket(unsigned int ms);
	bool TicketCompleted(unsigned int tic);
private:
	unsigned int timer_ids;
	Timer(void) {
		timer_ids=0;
	}
	std::vector<Ticket> tickets;
};
