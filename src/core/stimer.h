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

	// Defines the action for the signal -> thus signalAction <img src="http://quirk.ch/wordpress/wp-includes/images/smilies/icon_wink.gif" alt=";-)" class="wp-smiley">
	struct sigaction signalAction;

	// The itimerspec structure for the timer
	struct itimerspec timerSpecs;

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
