/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STIMER_H
#define STIMER_H

#include "stime.h"

/*!
	\brief Timer class with HIGH Resolution Posix timer
	\todo to implement
*/
class STimer
{
private:
	bool isRunning;
	STimeHighRes interval;

public:
    STimer();
};

#endif // STIMER_H
