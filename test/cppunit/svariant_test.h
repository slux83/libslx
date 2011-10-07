/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SVARIANT_TEST_H
#define SVARIANT_TEST_H

#include <cstring>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/core/svariant.h"

/*!
	\brief Test Unit for SWaitCondition object
*/
class SVariantTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SVariantTest);
CPPUNIT_TEST(testVariant);
CPPUNIT_TEST_SUITE_END();

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

};

#endif // SVARIANT_TEST_H
