/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STIMER_H
#define STIMER_H

#include <signal.h>
#include <time.h>
#include "stime.h"
#include "ssignal.h"
#include "../concurrent/smutex.h"

/*!
	\brief Timer class with HIGH Resolution Posix timer
	\note All functions are thread-safe
*/
class STimer
{
private:
	//! runnnig flag
	bool isRunning;

	//! timeout interval
	struct itimerspec interval;

	//! number of times the timeout occurs. Default 0 means forever.single timeout flag. If true the timeout signal will be called only the first time
	uint32_t timeoutCount;

	//! Internal counter
	uint32_t fireCounter;

	/*! Callback called by the POSIX timer after the timeout */
	static void timerHandler(int signalType, siginfo_t *sigInfo, void *context);

	//! the posix timer ID
	timer_t timerId;

	//! init flag
	bool init;

	// Signal blocking set
	sigset_t SigBlockSet;

	// The according signal event containing the this-pointer
	struct sigevent signalEvent;

	// Defines the action for the signal
	struct sigaction signalAction;

	// The itimerspec structure for the timer
	struct itimerspec timerSpecs;

	//! Internal mutex
	SMutex mutex;

	//! Getter for timeoutCount
	uint32_t getfireCounter();

	//! Add 1 to fireCounter
	void encreaseFireCounter();

public:
	//! Constructor
    STimer();

	//! Distructor
	virtual ~STimer();

	/*! Setter for interval.
		\param the interval
		\note the new interval will be used in the next start()
		\warning if the interval is 0, this will be setted to 1 nanosec. Use stop() to stop the timer.
	*/
	void setInterval(const STimeHighRes &interval);

	/*! Setter for interval.
		\param the interval with a minour precision.
		\note the new interval will be used in the next start()
		\warning if the interval is 0, this will be setted to 1 usec. Use stop() to stop the timer.
	*/
	void setInterval(const STimestamp &interval);

	/*! Setter for timeoutCount.
		\param count
		\note 0 means forever
	*/
	void setTimeoutCount(uint32_t count);

	/*! Timeout signal */
	SSignal0 *timeout;

	//! Arm and start the timer
	void start();

	//! Disarm and stop the timer
	void stop();

	//! Getter for timeoutCount
	uint32_t getTimeoutCount();
};

#endif // STIMER_H
