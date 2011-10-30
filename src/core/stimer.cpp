/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include <signal.h>
#include <time.h>
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
	if (timer_create(CLOCK_REALTIME, NULL, &timerId) == -1)
	{
		sWarning("STimer::STimer() Cannot create timer. ERRNO=%d", errno);
		init = false;
	}
	else
	{
		init = true;
	}

	if (init)
	{
		//TODO
		//timer_connect(timerId, &STimer::timerHandler, 0);
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

void STimer::timerHandler(timer_t timerId, int arg)
{
	S_USE_VAR(arg);
	S_USE_VAR(timerId);

	if (timeout)
		timeout->fire();

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

