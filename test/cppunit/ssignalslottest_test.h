/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALSLOTTEST_TEST_H
#define SSIGNALSLOTTEST_TEST_H

#include <unistd.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/core/ssignal.h"
#include "../../src/concurrent/sthread.h"
#include "../../src/core/stime.h"

/*!
	\brief Test Unit for Signal/Slot feature
*/
class SSignalSlotTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SSignalSlotTest);
CPPUNIT_TEST(testSingleThread);
CPPUNIT_TEST(testMultiThread);
CPPUNIT_TEST(testSingleThreadAsync);
//CPPUNIT_TEST(testBenchmark);
CPPUNIT_TEST_SUITE_END();

private:

	class MyThread1 : public SThread
	{
	private:
		SSignal1<unsigned int> *s1;

	public:
		MyThread1(SSignal1<unsigned int> *_s1)
		{
			s1 = _s1;
		}

	protected:
		void run()
		{
			for (unsigned int i=0; i< 10000000; ++i)
				s1->fire(i);
		}

	};

	//
	class MyThread2 : public SThread
	{
	private:
		SSignal1<const char*> *s1;

	public:
		MyThread2(SSignal1<const char*> *_s1)
		{
			s1 = _s1;
		}

	protected:
		void run()
		{
			for (unsigned int i=0; i< 10000000; ++i)
				s1->fire("hello");
		}

	};

	//Internal Slot class
	class MySlotClass2 : public SSlot
	{
	public:
		int globalVar;

		MySlotClass2()  { globalVar = 0; }

		void slot0(int a) { globalVar += a; }
		void slot1(int a) { globalVar += a; }
		void slot2(int a) { globalVar += a; }
		void slot3(int a) { globalVar += a; }
		void slot4(int a) { globalVar += a; }
		void slot5(int a) { globalVar += a; }
		void slot6(int a) { globalVar += a; }
		void slot7(int a) { globalVar += a; }
		void slot8(int a) { globalVar += a; }
		void slot9(int a) { globalVar += a; }
	};

	//Internal Slot class
	class MySlotClass1 : public SSlot
	{
	public:
		int slot0Counter;
		int slot0bisCounter;
		int slot1Counter;

		MySlotClass1()
		{
			slot0Counter = 0;
			slot0bisCounter = 0;
			slot1Counter = 0;
		}

		void resetCounters()
		{
			slot0Counter = 0;
			slot0bisCounter = 0;
			slot1Counter = 0;
		}

		void mySlot0()
		{
			slot0Counter++;
#if 0
			std::cout << "MySlotClass1::mySlot0()" << std::endl;
#endif
		}

		void mySlot0bis()
		{
			slot0bisCounter++;
#if 0
			std::cout << "MySlotClass1::mySlot0bis()" << std::endl;
#endif
		}

		void mySlot1(const char* arg1)
		{
			slot1Counter++;
#if 0
			std::cout << "MySlotClass1::mySlot1(const char* arg1='" << arg1
					  << "')" << std::endl;
#endif
		}

		//Overload
		void mySlot1(unsigned int arg1)
		{
			slot1Counter++;
#if 0

			std::cout << "MySlotClass1::mySlot1(unsigned int arg1=" << arg1
					  << ")" << std::endl;
#endif
		}

		void mySleepSlot(int arg1)
		{
			std::cout << "[Thread " << SThread::getCurrentThreadId() <<
						 "] MySlotClass1::mySleepSlot(" << arg1 << ")" << std::endl;
			fflush(stdout);
			usleep(1000000);
		}
	};

	MySlotClass1 *mySlot1;
	SSignal0 mySignal0;
	SSignal1<const char*> *mySignal1;
	SSignal1<unsigned int> *mySignal1bis;

public:

	void setUp()
	{
		mySignal1 = new SSignal1<const char*>();
		mySignal1bis = new SSignal1<unsigned int>();
		mySlot1 = new MySlotClass1();

		bool connectionEsite = false;

		connectionEsite = mySignal0.connect(mySlot1, &MySlotClass1::mySlot0);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);

		connectionEsite = mySignal0.connect(mySlot1, &MySlotClass1::mySlot0bis);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);

		connectionEsite = mySignal1->connect(mySlot1, &MySlotClass1::mySlot1);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);

		connectionEsite = mySignal1bis->connect(mySlot1, &MySlotClass1::mySlot1);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);
	}

	void tearDown()
	{
		delete mySignal1;
		mySignal1 = NULL;

		delete mySignal1bis;
		mySignal1bis = NULL;

		delete mySlot1;
		mySlot1 = NULL;


		fflush(stdout);
	}

	void testSingleThread()
	{
		//Reset counters
		mySlot1->resetCounters();

		mySignal0.fire();	//as invocation
		mySignal0();		//as functor

		mySignal1->fire("Hello");	//as invocation
		(*mySignal1)("Hello");		//as functor

		mySignal1bis->fire(1);

		//Test implicit conversion
		mySignal1bis->fire(12.8f);

		//Test overflow
		mySignal1bis->fire(-1);

		CPPUNIT_ASSERT_EQUAL(mySlot1->slot0Counter, 2);
		CPPUNIT_ASSERT_EQUAL(mySlot1->slot0bisCounter, 2);
		CPPUNIT_ASSERT_EQUAL(mySlot1->slot1Counter, 5);
	}

	void testMultiThread()
	{
		//Reset counters
		mySlot1->resetCounters();

		//Create stress threads
		MyThread1 mt1(mySignal1bis);
		MyThread2 mt2(mySignal1);

		//Run the test
		mt1.start();
		mt2.start();

		//Wait...
		mt1.join();
		mt2.join();

		//Each thread calls the slot 10M times
		CPPUNIT_ASSERT_EQUAL(mySlot1->slot1Counter, 20000000);
	}

	void testSingleThreadAsync()
	{
		MySlotClass1 mySlot;
		SSignal1<int> *ssignal = new SSignal1<int>(SSignalFlagThreadSafe | SSignalFlagAsyncConnection);

		ssignal->connect(&mySlot, &MySlotClass1::mySleepSlot);

		for (int i=0; i<30; ++i)
			ssignal->fire(i);

		std::cout << "Good night... THREAD=" << SThread::getCurrentThreadId() << std::endl;
		sleep(30);
	}

	void testBenchmark()
	{
		SSignal1<int> s1;

		STimestamp t0 = STime::now();
		for (int i=0; i<10000000; ++i)
			s1.fire(12);

		STimestamp diff = STime::diff(t0);

		std::cout << std::endl << "1. DIFF: sec=" << diff.sec << " usec=" << diff.usec << std::endl;
		fflush(stdout);

		MySlotClass2 msc2;

		s1.connect(&msc2, &MySlotClass2::slot0);
		s1.connect(&msc2, &MySlotClass2::slot1);
		s1.connect(&msc2, &MySlotClass2::slot2);
		s1.connect(&msc2, &MySlotClass2::slot3);
		s1.connect(&msc2, &MySlotClass2::slot4);

		t0 = STime::now();

		for (int i=0; i<10000000; ++i)
			s1.fire(1);

		diff = STime::diff(t0);

		std::cout << std::endl << "2. DIFF: sec=" << diff.sec << " usec=" << diff.usec << std::endl;
		fflush(stdout);

		s1.connect(&msc2, &MySlotClass2::slot5);
		s1.connect(&msc2, &MySlotClass2::slot6);
		s1.connect(&msc2, &MySlotClass2::slot7);
		s1.connect(&msc2, &MySlotClass2::slot8);
		s1.connect(&msc2, &MySlotClass2::slot9);

		t0 = STime::now();

		for (int i=0; i<10000000; ++i)
			s1.fire(1);

		diff = STime::diff(t0);

		std::cout << std::endl << "3. DIFF: sec=" << diff.sec << " usec=" << diff.usec << std::endl;
		fflush(stdout);
	}
};

#endif // SSIGNALSLOTTEST_TEST_H
