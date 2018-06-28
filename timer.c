#include "timer.h"
static const long billion = 1000000000L;

static void addNanosToTimespec
	(struct timespec* ts, long nanos)
{
	ts->tv_nsec += nanos;
	ts->tv_sec += ts->tv_nsec / billion;
	ts->tv_nsec %= billion;
}

static bool isGreaterThan
	(struct timespec* a, struct timespec* b)
{
	if(a->tv_sec > b->tv_sec)return true;
	if(a->tv_sec < b->tv_sec)return false;
	if(a->tv_nsec > b->tv_nsec) return true;
	return false;
}

Timer constructTimer(long delay)
{

	Timer t = {.delay = 1000000*delay};
	timespec_get(&(t.alarm), TIME_UTC);
	addNanosToTimespec(&(t.alarm), t.delay);
	return t;
}

// Return true if time for this to happen)
bool checkTimer(Timer* timer)
{
	struct timespec current_time;
	timespec_get(&current_time, TIME_UTC);

	if (isGreaterThan(&current_time, &(timer->alarm)))
	{
		timespec_get(&(timer->alarm), TIME_UTC);
		addNanosToTimespec(&(timer->alarm), timer->delay);
		return true;
	}
	return false;
}
