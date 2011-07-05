/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SBLOCKINGQUEUE_TEST_H
#define SBLOCKINGQUEUE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/sblockingqueue.h"
#include "../../src/concurrent/sthread.h"

/*!
	\brief Test Unit for SBlockingQueue object
*/
class SBlockingQueueTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SBlockingQueueTest);
CPPUNIT_TEST(testOneConsumerOneProducer);
CPPUNIT_TEST(testOneConsumerNProducer);
CPPUNIT_TEST_SUITE_END();

private:
	//The object to test
	static SBlockingQueue<int> *queue;

	//Internal test thread class -------------------------------------- [BEGIN]
	class TestThread : public SThread
	{
	public:
		int startNum;
		int numberOfDequeues;

		TestThread(int n, const std::string &name) : SThread(name)
		{
			startNum = n;
		}

	protected:
		virtual void run()
		{
			SThread::run();

			//Enqueue/Dequeue into the queue 10 elements
			for (int i=0; i<10; ++i)
			{

				queue->enqueue(startNum + i);
#if 0
				std::cout << name << ": Enqueueing " << (startNum + i) << std::endl;
				fflush(stdout);
#endif
			}
		}
	};
	//Internal test thread class ---------------------------------------- [END]

public:
	void setUp()
	{
		queue = new SBlockingQueue<int>();
	}

	void tearDown()
	{
		delete queue;
		queue = NULL;

		fflush(stdout);
	}

	void testOneConsumerOneProducer()
	{
		//Ensure that the queue is empty
		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());

		//This thread will be insert into the queue values from 0...9
		TestThread *producer = new TestThread(0, "Thread-producer");

		//Start the thread
		producer->start();

		//Now this thread (the consumer) will consume ten values
		for (int i=0; i<10; ++i)
		{
			//The queue shall mantains the same insertion order during the dequeueing
			CPPUNIT_ASSERT_EQUAL(i, queue->dequeue());
		}

		producer->join();

		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());

		delete producer;
		producer = NULL;
	}

	void testOneConsumerNProducer()
	{
		//Ensure that the queue is empty
		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());

		//We create 3 producer therads. Each of these will insert into the queue 10 elements.
		TestThread *producer1 = new TestThread(0,  "Thread-producer-1");
		TestThread *producer2 = new TestThread(10, "Thread-producer-2");
		TestThread *producer3 = new TestThread(20, "Thread-producer-3");

		//Start the first thread
		producer1->start();

		//Now this thread (the consumer) will consume 30 values
		for (int i=0; i<30; ++i)
		{
			//We cannot know with value is dequeued, but we can test
			//that its range is between 0 and 29 :)
			int dequeuedVal = queue->dequeue();
#if 0
				std::cout << "Main: Dequeueing " << dequeuedVal << std::endl;
				fflush(stdout);
#endif
			CPPUNIT_ASSERT_EQUAL(true, (dequeuedVal >= 0 && dequeuedVal <= 29));

			//After the first 6 values consumed, we let start the other two threads :p
			if (i == 5)
			{
				producer2->start();
				producer3->start();
			}
		}

		producer1->join();
		producer2->join();
		producer3->join();

		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());

		delete producer1;
		producer1 = NULL;

		delete producer2;
		producer2 = NULL;

		delete producer3;
		producer3 = NULL;
	}
};

#endif // SBLOCKINGQUEUE_TEST_H
