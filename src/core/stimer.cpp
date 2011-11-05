/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include <cstring>
#include "stimer.h"

STimer::STimer()
{
	interval.it_interval.tv_sec = 0;
	interval.it_interval.tv_nsec = 0;
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_nsec = 0;

	isRunning = false;
	isSingleTimeout = false;

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
	this->interval.it_value.tv_sec = interval.sec;
	this->interval.it_value.tv_nsec = interval.nsec;

	this->interval.it_interval.tv_sec = interval.sec;
	this->interval.it_interval.tv_nsec = interval.nsec;
}

void STimer::setInterval(const STimestamp &interval)
{
	this->interval.it_value.tv_sec = interval.sec;
	this->interval.it_value.tv_nsec = interval.usec * 1000;

	this->interval.it_interval.tv_sec = interval.sec;
	this->interval.it_interval.tv_nsec = interval.usec * 1000;
}

//FIXME/TODO never invoked!!
void STimer::timerHandler(int signalType, siginfo_t *sigInfo, void *context)
{
	S_USE_VAR(signalType);
	S_USE_VAR(context);

	// get the pointer out of the siginfo structure and asign it to a new pointer variable
	STimer *timerCaller = reinterpret_cast<STimer*> (sigInfo->si_value.sival_ptr);

	// call the member function
	if (timerCaller->timeout)
		timerCaller->timeout->fire();

}

void STimer::setIsSingleTimeout(bool isSingle)
{
	isSingleTimeout = isSingle;

	if (isSingleTimeout)
	{
		this->interval.it_interval.tv_sec = 0;
		this->interval.it_interval.tv_nsec = 0;
	}
	else
	{
		this->interval.it_interval.tv_sec = this->interval.it_value.tv_sec;
		this->interval.it_interval.tv_nsec = this->interval.it_value.tv_nsec;
	}
}

void STimer::start()
{
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

	if (timer_settime(timerId, 0, &interval, NULL) == -1)
	{
		sWarning("STimer::start() Cannot arm timer. ERRNO=%d", errno);
	}
}

void STimer::stop()
{
	if (!init)
	{
		sWarning("STimer::start() Timer init failed.");
		return;
	}

	if (!isRunning)
	{
		sWarning("STimer::start() Timer not running.");
		return;
	}

	uint32_t tmpSec = interval.it_value.tv_sec;
	uint64_t tmpNSec = interval.it_value.tv_nsec;


	//Disarm
	interval.it_value.tv_sec = 0;
	interval.it_value.tv_nsec = 0;
	if (timer_settime(timerId, 0, &interval, NULL) == -1)
	{
		sWarning("STimer::start() Cannot arm timer. ERRNO=%d", errno);
	}

	interval.it_value.tv_sec = tmpSec;
	interval.it_value.tv_nsec = tmpNSec;
}

