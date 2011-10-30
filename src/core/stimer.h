/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STIMER_H
#define STIMER_H

#include "stime.h"
#include "ssignal.h"

/*!
	\brief Timer class with HIGH Resolution Posix timer
	\todo to implement
*/
class STimer
{
private:
	//! runnnig flag
	bool isRunning;

	//! timeout interval
	struct itimerspec interval;

	//! single timeout flag. If true the timeout signal will be called only the first time
	bool isSingleTimeout;

	/*! Callback called by the POSIX timer after the timeout
		\param timerId the ID of the timer
		\param arg the argument
	*/
	void timerHandler(timer_t timerId, int arg);

	//! the posix timer ID
	timer_t timerId;

	//! init flag
	bool init;

public:
	//! Constructor
    STimer();

	//! Distructor
	virtual ~STimer();

	/*! Setter for interval. \param the interval */
	void setInterval(const STimeHighRes &interval);

	/*! Setter for interval. \param the interval with a minour precision */
	void setInterval(const STimestamp &interval);

	/*! Setter for isSingleTimeout. \param isSingle */
	void setIsSingleTimeout(bool isSingle);

	/*! Timeout signal */
	SSignal0 *timeout;

	//! Arm and start the timer
	void start();

	//! Disarm and stop the timer
	void stop();
};

#endif // STIMER_H
