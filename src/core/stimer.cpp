/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include <cstring>
#include "stimer.h"
#include "../concurrent/smutexlocker.h"

STimer::STimer()
{
	interval.it_interval.tv_sec = 0;
	interval.it_interval.tv_nsec = 0;
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_nsec = 0;

	isRunning = false;
	timeoutCount = 0;
	fireCounter = 0;

	timeout = new SSignal0();
	timerId = 0;

	init = false;

	// Clear the sa_mask
	sigemptyset(&signalAction.sa_mask);

	// set the SA_SIGINFO flag to use the extended signal-handler function
	signalAction.sa_flags = SA_SIGINFO;

	// Define sigaction method
	// This function will be called by the signal
	signalAction.sa_sigaction = &STimer::timerHandler;
	//signal(SIGALRM, &STimer::timerHandler2);	//TODO FIX THIS SHIT!

	// Define sigEvent
	// This information will be forwarded to the signal-handler function
	memset(&signalEvent, 0, sizeof(signalEvent));

	// With the SIGEV_SIGNAL flag we say that there is sigev_value
	signalEvent.sigev_notify = SIGEV_SIGNAL;

	// Now it's possible to give a pointer to the object
	signalEvent.sigev_value.sival_ptr = (void*) this;

	// Declare this signal as Alarm Signal
	signalEvent.sigev_signo = SIGALRM;

	if (sigaction(SIGALRM, &signalAction, NULL) == -1)
		sWarning("STimer::STimer() Could not install new signal handler. ERRNO=%d", errno);
	else
		init = true;

	if (init && timer_create(CLOCK_REALTIME, &signalEvent, &timerId) == -1)
	{
		sWarning("STimer::STimer() Cannot create timer. ERRNO=%d", errno);
		init = false;
	}
	else
	{
		init = true;
	}
}

STimer::~STimer()
{
	if (NULL != timeout)
	{
		delete timeout;
		timeout = NULL;
	}
}

void STimer::setInterval(const STimeHighRes &interval)
{
	SMutexLocker l(&mutex); S_USE_VAR(l);

	int adjust = 0;
	if (interval.nsec == 0 && interval.sec == 0)
	{
		sWarning("STimer::setInterval() interval is 0. Adjusted to 1 nanosec.");
		adjust = 1;
	}

	this->interval.it_value.tv_sec = interval.sec;
	this->interval.it_value.tv_nsec = interval.nsec + adjust;

	this->interval.it_interval.tv_sec = interval.sec;
	this->interval.it_interval.tv_nsec = interval.nsec + adjust;
}

void STimer::setInterval(const STimestamp &interval)
{
	SMutexLocker l(&mutex); S_USE_VAR(l);

	int adjust = 0;
	if (interval.usec == 0 && interval.sec == 0)
	{
		sWarning("STimer::setInterval() interval is 0. Adjusted to 1 usec.");
		adjust = 1;
	}

	this->interval.it_value.tv_sec = interval.sec;
	this->interval.it_value.tv_nsec = (interval.usec + adjust) * 1000;

	this->interval.it_interval.tv_sec = interval.sec;
	this->interval.it_interval.tv_nsec = (interval.usec + adjust) * 1000;
}

void STimer::timerHandler(int signalType, siginfo_t *sigInfo, void *context)
{
	S_USE_VAR(signalType);
	S_USE_VAR(context);

	S_ASSERT_MSG(sigInfo->si_value.sival_ptr != NULL, "STimer::timerHandler() NULL sigInfo->si_value.sival_ptr");

	// get the pointer out of the siginfo structure and asign it to a new pointer variable
	STimer *timerCaller = reinterpret_cast<STimer*> (sigInfo->si_value.sival_ptr);

	//check if we have to stop the timer
	if (timerCaller->timeoutCount != 0)
	{
		if (timerCaller->getfireCounter() == timerCaller->getTimeoutCount())
		{
			timerCaller->stop();
			return;
		}
		else
			timerCaller->encreaseFireCounter();
	}

	// call the member function
	if (timerCaller->timeout)
		timerCaller->timeout->fire();

}

void STimer::setTimeoutCount(uint32_t count)
{
	SMutexLocker l(&mutex); S_USE_VAR(l);

	timeoutCount = count;
}

void STimer::start()
{
	SMutexLocker l(&mutex); S_USE_VAR(l);

	if (!init)
	{
		sWarning("STimer::start() Timer init failed.");
		return;
	}

	if (isRunning)
	{
		sWarning("STimer::start() Timer already running.");
		return;
	}

	fireCounter = 0;

	if (timer_settime(timerId, 0, &interval, NULL) == -1)
	{
		sWarning("STimer::start() Cannot arm timer. ERRNO=%d", errno);
	}
	else
	{
		isRunning = true;
	}
}

void STimer::stop()
{
	SMutexLocker l(&mutex); S_USE_VAR(l);

	if (!init)
	{
		sWarning("STimer::stop() Timer init failed.");
		return;
	}

	if (!isRunning)
	{
		sWarning("STimer::stop() Timer not running.");
		return;
	}

	uint32_t tmpSec = interval.it_value.tv_sec;
	uint64_t tmpNSec = interval.it_value.tv_nsec;


	//Disarm
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_nsec = 0;
	if (timer_settime(timerId, 0, &interval, NULL) == -1)
	{
		sWarning("STimer::stop() Cannot arm timer. ERRNO=%d", errno);
	}

	interval.it_value.tv_sec = tmpSec;
	interval.it_value.tv_nsec = tmpNSec;
}

uint32_t STimer::getfireCounter()
{
	SMutexLocker l(&mutex); S_USE_VAR(l);
	return fireCounter;
}

void STimer::encreaseFireCounter()
{
	SMutexLocker l(&mutex); S_USE_VAR(l);
	fireCounter++;
}

uint32_t STimer::getTimeoutCount()
{
	SMutexLocker l(&mutex); S_USE_VAR(l);
	return timeoutCount;
}
