/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SBLOCKINGQUEUE_TEST_H
#define SBLOCKINGQUEUE_TEST_H

#include <unistd.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/sunboundedblockingqueue.h"
#include "../../src/concurrent/sthread.h"

//Used to count dequeues in test testNConsumerNProducer()
SMutex m1;
int dequeues;

/*!
	\brief Test Unit for SUnboundedBlockingQueue object
*/
class SUnboundedBlockingQueueTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SUnboundedBlockingQueueTest);
CPPUNIT_TEST(testOneConsumerOneProducer);
CPPUNIT_TEST(testOneConsumerNProducer);
CPPUNIT_TEST(testNConsumerNProducer);
CPPUNIT_TEST_SUITE_END();

private:
	//The object to test
	static SUnboundedBlockingQueue<int> *queue;

	//Internal test thread class -------------------------------------- [BEGIN]
	class TestThread : public SThread
	{
	public:
		int startNum;
		bool isProducer;		//I'm damn lazy :p

		TestThread(int n, const std::string &name, bool producer) : SThread(name)
		{
			startNum = n;
			isProducer = producer;
		}

	protected:
		virtual void run()
		{
			SThread::run();

			//Enqueue/Dequeue into the queue 10 elements
			if (isProducer)
			{
				for (int i=0; i<10; ++i)
				{
					queue->enqueue(startNum + i);
#if 0
					std::cout << name << ": Enqueueing " << (startNum + i) << std::endl;
					fflush(stdout);
#endif
				}
			}
			else
			{
				while (true)
				{
					int dequeuedVal = queue->dequeue();
					m1.lock();
					++dequeues;
					CPPUNIT_ASSERT_EQUAL(true, (dequeuedVal >= 0 && dequeuedVal <= 29));
#if 0
					std::cout	<< name << ": Dequeueing " << (dequeuedVal)
								<< " dequeues: " << dequeues << std::endl;
					fflush(stdout);
#endif

					if (dequeues == 29)	//Okay, all datas are dequeued
					{
						m1.unlock();
						break;
					}
					m1.unlock();
				}
			}
		}
	};
	//Internal test thread class ---------------------------------------- [END]

public:
	void setUp()
	{
		queue = new SUnboundedBlockingQueue<int>();
		dequeues = 0;
	}

	void tearDown()
	{
		//delete queue;
		//NOTE: we can't destroy here the queue. This is just a test :)
		fflush(stdout);
	}

	void testOneConsumerOneProducer()
	{
		delete queue;
		queue = new SUnboundedBlockingQueue<int>();

		//This thread will be insert into the queue values from 0...9
		TestThread *producer = new TestThread(0, "Thread-producer", true);

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
		TestThread *producer1 = new TestThread(0,  "Thread-producer-1", true);
		TestThread *producer2 = new TestThread(10, "Thread-producer-2", true);
		TestThread *producer3 = new TestThread(20, "Thread-producer-3", true);

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

	void testNConsumerNProducer()
	{
		//Ensure that the queue is empty
		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());

		//We create 3 producer therads. Each of these will insert into the queue 10 elements.
		TestThread *producer1 = new TestThread(0,  "Thread-producer-1", true);
		TestThread *producer2 = new TestThread(10, "Thread-producer-2", true);
		TestThread *producer3 = new TestThread(20, "Thread-producer-3", true);

		//And... we create 3 consumer threads. Each of these will read a maximum of 10 elements.
		//This is only a test case, not the reality
		TestThread *consumer1 = new TestThread(0,  "Thread-consumer-1", false);
		TestThread *consumer2 = new TestThread(0,  "Thread-consumer-2", false);
		TestThread *consumer3 = new TestThread(0,  "Thread-consumer-3", false);

		//Let's the three consumers wait for data...
		consumer1->start();
		consumer2->start();
		consumer3->start();

		//Now, let's rock!
		producer1->start();
		producer2->start();
		producer3->start();

		producer1->join();
		producer2->join();
		producer3->join();

		//We wait 1 sec that should be enough for consumers.
		sleep(1);

		CPPUNIT_ASSERT_EQUAL(true, queue->isEmpty());
		CPPUNIT_ASSERT_EQUAL(30, dequeues);
#if 0
		std::cout << "Dequeues: " << dequeues << std::endl;
#endif
		//For definition consumer threads have an infinite internal loop that wait data forever

		delete producer1;
		producer1 = NULL;

		delete producer2;
		producer2 = NULL;

		delete producer3;
		producer3 = NULL;

		delete consumer1;
		consumer1 = NULL;

		delete consumer2;
		consumer2 = NULL;

		delete consumer3;
		consumer3 = NULL;
	}
};

#endif // SBLOCKINGQUEUE_TEST_H
