/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/


#ifndef SSERIALIZABLE_TEST_H
#define SSERIALIZABLE_TEST_H

#include <cstring>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/io/sserializable.h"

/*!
	\brief Test Unit for SSerializable
*/
class SSerializableTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SSerializableTest);
CPPUNIT_TEST(testSerilizableObject);
CPPUNIT_TEST_SUITE_END();

private:
	class MySerializableObj : public SSerializable
	{
		S_SERIALIZABLE_ID(1000)
		S_SERIALIZABLE_INT8(myInt8)

	private:
		s_int8 myInt8;

	public:
		void setMyInt8(s_int8 val)
		{
			myInt8 = val;
		}
	};

public:
	void setUp()
	{
	}

	void tearDown()
	{
		fflush(stdout);
	}

	void testSerilizableObject()
	{
		MySerializableObj obj;
		obj.setMyInt8(12);
		CPPUNIT_ASSERT_EQUAL(12, (int)obj.__getSerializable_myInt8());
		CPPUNIT_ASSERT_EQUAL(1000, (int)obj.__getSerializableId());

	}
};

#endif // SSERIALIZABLE_TEST_H
