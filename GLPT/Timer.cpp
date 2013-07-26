#include "Timer.h"

Timer* GLPT_timer;

bool Timer::TicketCompleted(unsigned int tic) {
	INT64 current_time;
	INT64 ticks_per_ms=tick_frequency/1000;
	QueryPerformanceCounter((LARGE_INTEGER*) &current_time);

	for(unsigned int i=0;i<tickets.size();i++) {
		if (tickets[i].ticket_id!=tic) continue;
		INT64 time_difference=current_time-tickets[i].start_time;
		if ((time_difference / ticks_per_ms) >= tickets[i].duration) {
			tickets.erase(tickets.begin() + i);
			return true;
		}
	}

	return false;
}

unsigned int Timer::CreateTicket(unsigned int ms) {
	Ticket new_ticket;
	INT64 current_time;
	QueryPerformanceCounter((LARGE_INTEGER*) &current_time);
	new_ticket.ticket_id=timer_ids++;
	new_ticket.duration=ms;
	new_ticket.start_time=current_time;
	tickets.push_back(new_ticket);

	return new_ticket.ticket_id;
}