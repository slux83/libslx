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
public:
	void consume(int i)
	{
		sDebug("EventConsumer::consume() consuming %d", i);
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
	EventConsumer ec1, ec2, ec3;

	ep.event->connect(&ec1, &EventConsumer::consume);
	ep.event->connect(&ec2, &EventConsumer::consume);
	ep.event->connect(&ec3, &EventConsumer::consume);
	ep.start();

	sDebug("Main sleeping...");
	programEnd.acquire(NUMBER_OF_FIRES * 3);	//We wait NUMBER_OF_FIRES async signal fires with 3 consumer
	sDebug("Main exiting...");
	return 0;
}
