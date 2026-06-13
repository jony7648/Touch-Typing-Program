#include "timer.h"

#include <pthread.h>
#include <unistd.h>

#define MAX_TIMERS 10

typedef struct {
	pthread_t thread;
	void (*timeout_func)(TimerSignalTimeout*);
	uint32_t seconds;
	bool in_use;
} Timer;


Timer G_thread_arr[MAX_TIMERS];

void *exec_on_timeout(void *arg) {
	Timer *p_timer = arg;

	if (!p_timer) {
		return NULL;
	}

	sleep(p_timer->seconds);

	TimerSignalTimeout *signal = NULL;

	p_timer->timeout_func(signal);

	p_timer->in_use = false;

	return NULL;
}

Timer *get_timer() {
	Timer *ret_timer = NULL;

	for (size_t i=0; i<MAX_TIMERS; i++) {
		Timer *p_timer = &G_thread_arr[i];

		if (!p_timer->in_use) {
			p_timer->in_use = true;
			ret_timer = p_timer;
			break;
		}
	}

	return ret_timer;
}

GCError timer_spawn(uint32_t seconds, void (timeout_func)(TimerSignalTimeout*)) {
	Timer *p_timer = get_timer();

	if (!p_timer) {
		printf("Was not able to grab a thread for the timer!\n");
		return ErrorNoThreadAvailable;
	}

	p_timer->seconds = seconds;
	p_timer->timeout_func = timeout_func;
	
	bool in_use;
	pthread_create(&p_timer->thread, NULL, exec_on_timeout, p_timer);

	return ErrorClear; 

}
