#include "Timer.h"

Timer* GLPT_timer;

uint64_t current_ms(void) {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

bool Timer::TicketCompleted(int64_t tic) {
	return (current_ms() >= tic);
}

int64_t Timer::CreateTicket(unsigned int ms) {
	return current_ms() + ms;
}
