/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "stime.h"
#include <cstdlib>

STime::STime()
{
	gettimeofday(&currentTime, NULL);
}

STimestamp STime::now()
{
	STimestamp ts;
	struct timeval tv;

	gettimeofday(&tv, NULL);
	ts.sec = tv.tv_sec;
	ts.usec = tv.tv_usec;

	return ts;
}

STimestamp STime::getCurrentTime()
{
	STimestamp ts;

	ts.sec = currentTime.tv_sec;
	ts.usec = currentTime.tv_usec;

	return ts;
}


STimestamp STime::diff(STimestamp &old)
{
	STimestamp current = STime::now();
	STimestamp differ;

	unsigned long sec = abs(old.sec - current.sec);
	unsigned long usec = abs(old.usec - current.usec);


	differ.sec = sec;
	differ.usec = usec;

	return differ;
}
