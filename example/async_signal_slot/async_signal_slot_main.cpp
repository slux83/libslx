/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "../../src/core/ssignal.h"
#include "../../src/core/sapplication.h"
#include "../../src/concurrent/ssemaphore.h"
#include "../../src/core/stime.h"

#define NUMBER_OF_FIRES 1000

SSemaphore programEnd;

class EventProducer : public SThread
{
public:
	SSignal2<int, double> *event;

	EventProducer()
	{
		event = new SSignal2<int, double>(SSignalFlagThreadUnsafe | SSignalFlagAsyncConnection);
	}

	virtual ~EventProducer()
	{
		delete event;
		event = NULL;
	}

	void run()
	{
		for (int i=0; i<NUMBER_OF_FIRES; i++)
		{
			double tmp = i * 3.14159265;
			sDebug("EventProducer::startProduceEvents() firing %d, %.5f", i, tmp);

			event->fire(i, tmp);
		}
	}
};

class EventConsumer : public SSlot
{
private:
	int consumerNumber;

public:

	EventConsumer(int number)
	{
		consumerNumber = number;
	}

	void consume(int i, double d)
	{
		sDebug("EventConsumer::consume() consuming %d, %.5f (%d)", i, d, consumerNumber);
		programEnd.release();
	}
};

/*
 Example of asynchronous signals and slots
*/
int main (int argc, char** argv)
{
	S_USE_VAR(argc);
	S_USE_VAR(argv);

	SApplication::init();
	SApplication::getInstance()->start();

	EventProducer ep;
	EventConsumer ec1(1), ec2(2), ec3(3);

	ep.event->connect(&ec1, &EventConsumer::consume);
	ep.event->connect(&ec2, &EventConsumer::consume);
	ep.event->connect(&ec3, &EventConsumer::consume);

	STimestamp checkpointBegin = STime::now();

	ep.start();

	sDebug("Main sleeping...");
	programEnd.acquire(NUMBER_OF_FIRES * 3);	//We wait NUMBER_OF_FIRES async signal fires with 3 consumer
	STimestamp checkpointEnd = STime::now();

	sDebug("Main exiting... Total execution time: %d sec and %06d usec",
		   abs(checkpointBegin.sec - checkpointEnd.sec),
		   abs(checkpointBegin.usec - checkpointEnd.usec));

	SApplication::getInstance()->shutdown();

	return 0;
}
