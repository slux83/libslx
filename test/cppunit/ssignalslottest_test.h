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
			std::cout << "MySlotClass1::mySlot0()" << std::endl;
		}

		void mySlot0bis()
		{
			std::cout << "MySlotClass1::mySlot0bis()" << std::endl;
		}

		void mySlot1(const char* arg1)
		{
			std::cout << "MySlotClass1::mySlot1(const char* arg1='" << arg1
					  << "')" << std::endl;
		}

		//Overload
		void mySlot1(unsigned int arg1)
		{
			std::cout << "MySlotClass1::mySlot1(unsigned int arg1=" << arg1
					  << ")" << std::endl;
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
		bool connectionEsite = false;

		connectionEsite = mySignal0.connect(&mySlot1, &MySlotClass1::mySlot0);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);

		connectionEsite = mySignal1->connect(&mySlot1, &MySlotClass1::mySlot1);
		CPPUNIT_ASSERT_EQUAL(connectionEsite, true);

		mySignal0.fire();	//as invocation
		mySignal0();		//as functor

		mySignal1->fire("Hello");	//as invocation
		(*mySignal1)("Hello");		//as functor
	}
};

#endif // SSIGNALSLOTTEST_TEST_H
