/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALSLOTTEST_TEST_H
#define SSIGNALSLOTTEST_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../src/core/ssignal.h"

/*!
	\brief Test Unit for Signal/Slot feature
*/
class SSignalSlotTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SSignalSlotTest);
CPPUNIT_TEST(testConnections);
CPPUNIT_TEST_SUITE_END();

private:
	//Internal Slot class
	class MySlotClass1 : public SSlot
	{
	public:
		void mySlot0()
		{
			sDebug("MySlotClass1::mySlot0()");
		}

		void mySlot0bis()
		{
			sDebug("MySlotClass1::mySlot0bis()");
		}

		void mySlot1(const char* arg1)
		{
			sDebug("MySlotClass1::mySlot1(const char* arg1=%s)", arg1);
		}

		//Overload
		void mySlot1(unsigned int arg1)
		{
			sDebug("MySlotClass1::mySlot1(unsigned int arg1=%d)", arg1);
		}
	};

	MySlotClass1 mySlot1;
	SSignal0 mySignal0;
	SSignal1<const char*> *mySignal1;
	SSignal1<unsigned int> *mySignal1bis;

public:
	void setUp()
	{
		mySignal1 = new SSignal1<const char*>();
		mySignal1bis = new SSignal1<unsigned int>();
	}

	void tearDown()
	{
		delete mySignal1;
		delete mySignal1bis;

		mySignal1 = NULL;
		mySignal1bis = NULL;

		fflush(stdout);
	}

	void testConnections()
	{
		CPPUNIT_ASSERT_EQUAL(mySignal0.connect(&mySlot1, &MySlotClass1::mySlot0), true);

		mySignal0.fire();	//as invocation
		mySignal0();		//as functor
	}
};

#endif // SSIGNALSLOTTEST_TEST_H
