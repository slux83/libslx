/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SVARIANT_TEST_H
#define SVARIANT_TEST_H


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

		int test1 = v1;
		float test1fail = v1;

		double test2 = v2;
		std::string test3 = v3;

		CPPUNIT_ASSERT_EQUAL(v1.isTypeOf<int>(), true);
		CPPUNIT_ASSERT_EQUAL(v1.isTypeOf<std::string>(), false);

		CPPUNIT_ASSERT_EQUAL(test1, 666);
		CPPUNIT_ASSERT_EQUAL(test1fail, 0.0f);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(test2, 666.666, 0.00001);

		CPPUNIT_ASSERT_EQUAL(test3, std::string("Hello World"));



	}

};

#endif // SVARIANT_TEST_H
