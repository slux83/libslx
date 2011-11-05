/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include "../../src/core/ssignal.h"
#include "../../src/core/stimer.h"
#include "../../src/core/stime.h"
#include "../../src/concurrent/ssemaphore.h"

class MySlot : public SSlot
{
public:
	void mySlot()
	{
		STimestamp now = STime::now();
		sDebug("MySlot::mySlot() sec=%d\tmsec=%d", now.sec, now.usec / 1000);
	}
};

/*
	Example of STimer usage
*/
int main (int argc, char** argv)
{
	S_USE_VAR(argc);
	S_USE_VAR(argv);

	SApplication::init();
	SApplication::getInstance()->start();

	STimer timer;

	timer.setInterval(STimestamp(1, 500000));
	timer.setIsSingleTimeout(false);

	MySlot slot;

	timer.timeout->connect(&slot, &MySlot::mySlot);
	timer.start();

	//Deadlock
	SSemaphore sem;
	sem.acquire(3);

	sDebug("Exiting...");
	return 0;
}
