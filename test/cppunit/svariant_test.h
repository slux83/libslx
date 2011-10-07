/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SVARIANT_TEST_H
#define SVARIANT_TEST_H

#include <cstring>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/core/ssignalcall.h"

/*!
	\brief Test Unit for SVariant and internalS::SSignalCall object
*/
class SVariantTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SVariantTest);
CPPUNIT_TEST(testVariant);
CPPUNIT_TEST(testSignalCall);
CPPUNIT_TEST_SUITE_END();

private:
	//Test Type
	typedef struct
	{
		int i;
		bool b;
		double d;
	} MyPrivateStruct;

	//Test Type
	class MyPrivateClass
	{
	public:
		std::string name;
	};

public:
	void setUp()
	{
	}

	void tearDown()
	{
	}

	void testVariant()
	{
		//integer
		SVariant v1(666);

		//double
		SVariant v2(666.666);

		//std::string
		SVariant v3(std::string("Hello World"));

		//char*
		char *_v4 = new char[64];
		sprintf(_v4, "hello world %d", 666);
		SVariant v4(_v4);

		int test1 = v1;
		int _p1 = v1;
		int _p2 = v1;
		int *p1 = &_p1;
		int *p2 = &_p2;

		//Check if the operator() returns a copy
		CPPUNIT_ASSERT_EQUAL(p1 != p2, true);

		double test2 = v2;
		std::string test3 = v3;

		char* test4 = v4;

		CPPUNIT_ASSERT_EQUAL(v1.isTypeOf<int>(), true);
		CPPUNIT_ASSERT_EQUAL(v1.isTypeOf<std::string>(), false);

		CPPUNIT_ASSERT_EQUAL(test1, 666);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(test2, 666.666, 0.00001);

		CPPUNIT_ASSERT_EQUAL(test3, std::string("Hello World"));

		CPPUNIT_ASSERT_EQUAL(strncmp(test4, "hello world 666", 64), 0);
		CPPUNIT_ASSERT_EQUAL(v4.isTypeOf<const char*>(), false);

		//Test copy and assignment operator
		SVariant t1(100);
		SVariant t2 = t1;	//copy constructor

		CPPUNIT_ASSERT_EQUAL((int)t1, (int)t2);

		SVariant t3(3.14);
		t2 = t3;			//assignment operator

		CPPUNIT_ASSERT_DOUBLES_EQUAL((double)t2, (double)t3, 0.000001);
	}

	void testSignalCall()
	{


		SVariant arg0(666);
		SVariant arg1(std::string("Hello"));
		SVariant arg2(true);

		MyPrivateStruct mps;
		mps.b = true;
		mps.i = 1024;
		mps.d = 3.14;
		SVariant arg3(mps);

		MyPrivateClass mpc;
		mpc.name = "MyPrivateClass";
		SVariant arg4(mpc);

		//Constructor argument is a fake object. NEVER USE IT!
		internalS::SSignalCall call(reinterpret_cast<internalS::SAbstractSignal*>(new int()));

		call.addArgument(arg0, 0);
		call.addArgument(arg1, 1);
		call.addArgument(arg2, 2);
		call.addArgument(arg3, 3);
		call.addArgument(arg4, 4);

		int _arg0 = call.getArgument(0);
		std::string _arg1 = call.getArgument(1);
		bool _arg2 = call.getArgument(2);
		MyPrivateStruct _arg3 = call.getArgument(3);
		MyPrivateClass _arg4 = call.getArgument(4);

		CPPUNIT_ASSERT_EQUAL(_arg0, 666);
		CPPUNIT_ASSERT_EQUAL(_arg1, std::string("Hello"));
		CPPUNIT_ASSERT_EQUAL(_arg2, true);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(_arg3.d, 3.14, 0.000001);
		CPPUNIT_ASSERT_EQUAL(_arg4.name, std::string("MyPrivateClass"));

	}

};

#endif // SVARIANT_TEST_H
