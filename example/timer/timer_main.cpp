/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include "../../src/core/ssignal.h"
#include "../../src/core/stimer.h"
#include "../../src/core/stime.h"
#include "../../src/concurrent/ssemaphore.h"

SSemaphore sem;

class MySlot : public SSlot
{
public:
	int counterSlow;
	int counterFast;

	void mySlotSlow()
	{
		counterSlow++;

		STimestamp now = STime::now();
		sDebug("MySlot::mySlotSlow() sec=%d\tmsec=%03d\tcounter=%03d\t[SLOW]", now.sec, now.usec / 1000, counterSlow);

		if (counterSlow == 4)
		{
			sDebug("MySlot::mySlotSlow() bye bye");
			sem.release(3);
		}
	}

	void mySlotFast()
	{
		STimestamp now = STime::now();
		sDebug("MySlot::mySlotFast() sec=%d\tmsec=%03d\tcounter=%03d", now.sec, now.usec / 1000, ++counterFast);
	}

	void mySlotApplicationAboutToQuit()
	{
		sWarning("MySlot::mySlotApplicationAboutToQuit() application is about to quit...");
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


	MySlot slot;
	slot.counterSlow = 0;
	slot.counterFast = 0;
	STimer timer1;
	STimer timer2;

	SApplication::getInstance()->aboutToQuit->connect(&slot, &MySlot::mySlotApplicationAboutToQuit);

	//timer1: period 1.5 sec for a total of 4 times
	timer1.setInterval(STimestamp(1, 500000));
	timer1.setTimeoutCount(4);
	timer1.timeout->connect(&slot, &MySlot::mySlotSlow);

	//timer2: period 0.1 sec for a total of 40 times
	timer2.setInterval(STimestamp(0, 100000));
	timer2.setTimeoutCount(40);
	timer2.timeout->connect(&slot, &MySlot::mySlotFast);

	timer1.start();
	timer2.start();

	//Deadlock the main thread :)
	sem.acquire(3);

	timer1.stop();
	timer2.stop();

	SApplication::getInstance()->shutdown();

	return 0;
}
