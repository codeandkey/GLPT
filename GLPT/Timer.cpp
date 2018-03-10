#include "Timer.h"

Timer* GLPT_timer;

uint64_t current_ms(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

bool Timer::TicketCompleted(unsigned int tic) {
	int64_t ct = current_ms();

	for(unsigned int i=0;i<tickets.size();i++) {
		if (tickets[i].ticket_id!=tic) continue;
		if (ct >= tickets[i].start_time + tickets[i].duration) {
			tickets.erase(tickets.begin() + i);
			return true;
		}
	}

	return false;
}

unsigned int Timer::CreateTicket(unsigned int ms) {
	Ticket new_ticket;
	new_ticket.ticket_id=timer_ids++;
	new_ticket.duration=ms;
	new_ticket.start_time=current_ms();

	return new_ticket.ticket_id;
}
