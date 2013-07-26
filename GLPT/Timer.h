#pragma once

#include "Library.h"

class Timer;
extern Timer* GLPT_timer;

struct Ticket {
	unsigned int ticket_id;
	INT64 duration;
	INT64 start_time;
};

class Timer {
public:
	static void Create(void) {GLPT_timer=new Timer;}
	static void Release(void) {delete GLPT_timer;}

	unsigned int CreateTicket(unsigned int ms);
	bool TicketCompleted(unsigned int tic);
private:
	unsigned int timer_ids;
	INT64 tick_frequency;
	Timer(void) {
		QueryPerformanceFrequency((LARGE_INTEGER*) &tick_frequency);
		timer_ids=0;
	}
	std::vector<Ticket> tickets;
};