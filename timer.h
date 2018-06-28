#pragma once
#include <stdbool.h>
#include <time.h>
// Represents a timer that becomes "ready" every "delay" seconds
// Do not construct this directly
typedef struct 
{
	long delay;// In nanoseconds
	struct timespec alarm;
} Timer;

// Create a timer with specified delay
Timer constructTimer(long delay/* in milliseconds*/);

// Checks if timer has become "ready." If so, return true and reset timer,
// otherwise return false
bool checkTimer(Timer* timer);
