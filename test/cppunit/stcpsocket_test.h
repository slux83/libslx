/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STCPSOCKETTEST_H
#define STCPSOCKETTEST_H

#include <stdio.h>
#include <string.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/net/stcpsocket.h"

#define getIndexData "get index.html\n"
#define getIndexDataSize 15

/*!
	\brief Test Unit for STcpSocket object
*/
class STcpSocketTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(STcpSocketTest);
CPPUNIT_TEST(testSocket);
CPPUNIT_TEST_SUITE_END();

private:
	STcpSocket *socket;

public:
	void setUp()
	{
		socket = new STcpSocket();
	}

	void tearDown()
	{
		delete socket;
		socket = NULL;

		fflush(stdout);
	}

	void testSocket()
	{
		SAbstractSocket::SocketError error = (SAbstractSocket::SocketError) -1;
		bool retVal = socket->connect("127.0.0.1", 80, &error);

		CPPUNIT_ASSERT_EQUAL(retVal, true);
		CPPUNIT_ASSERT_EQUAL(error, SAbstractSocket::SocketErrorUnknown);

		CPPUNIT_ASSERT_EQUAL(socket->isConnected(), true);

		int64_t writtenBytes = socket->writeData(getIndexData, getIndexDataSize);

		CPPUNIT_ASSERT_EQUAL(writtenBytes, (int64_t) getIndexDataSize);

		char buff[4096];

		::memset(buff, 0, 4096);

		sleep(4);

		int64_t readBytes = socket->readData(buff, 4095);

		CPPUNIT_ASSERT_EQUAL(true, readBytes != -1);
	}
};

#endif // STCPSOCKETTEST_H
