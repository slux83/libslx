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
	SSignal1<int> *event;

	EventProducer()
	{
		event = new SSignal1<int>(SSignalFlagThreadUnsafe | SSignalFlagAsyncConnection);
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
			sDebug("EventProducer::startProduceEvents() firing %d", i);

			event->fire(i);
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

	void consume(int i)
	{
		sDebug("EventConsumer::consume() consuming %d (%d)", i, consumerNumber);
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

	return 0;
}
