/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STIME_H
#define STIME_H

#include <sys/time.h>
#include <unistd.h>

/*!
	\brief Timestamp class
*/
class STimestamp
{
public:
	STimestamp()
	{
		sec = 0;
		usec = 0;
	}

	unsigned long sec;
	unsigned long usec;
};

/*!
	\brief Time class
*/
class STime
{
private:
	//! the current time initialized in STime() constructor
	struct timeval currentTime;

public:
    STime();

	//! \return  the system time
	static STimestamp now();

	//! \return the currentTime value (initialized at creation time)
	STimestamp getCurrentTime();

	//! \return the time different between the current time and the passed time old.
	static STimestamp diff(STimestamp &old);
};

#endif // STIME_H
