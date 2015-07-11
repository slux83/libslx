/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SLOGHANDLER_TEST_H
#define SLOGHANDLER_TEST_H

#include <cstring>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/global/sglobal.h"

/*!
	Own log handler
*/
static void myLogHandler(SMsgLevel level, const char *msg)
{
	bool printMsg = false;
	switch (level)
	{
	case SMsgLevelDebug:
		if (printMsg)
		{
			std::cerr << "DEBUG:    " << msg << std::endl;
			fflush(stderr);
		}
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Debug", strcmp("debug", msg), 0);
		break;

	case SMsgLevelWarning:

		if (printMsg)
		{
			std::cerr << "WARNING:  " << msg << std::endl;
			fflush(stderr);
		}
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Warning", strcmp("warning", msg), 0);
		break;

	case SMsgLevelCritical:
		if (printMsg)
		{
			std::cerr << "CRITICAL: " << msg << std::endl;
			fflush(stderr);
		}
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Critical", strcmp("critical", msg), 0);
		break;

	case SMsgLevelFatal:
		if (printMsg)
		{
			std::cerr << "FATAL:    " << msg << std::endl;
			fflush(stderr);
		}
		CPPUNIT_ASSERT_EQUAL_MESSAGE("Fatal", strcmp("fatal", msg), 0);
		break;

	default:
		if (printMsg)
		{
			std::cerr << "DEFAULT:  " << msg << std::endl;
			fflush(stderr);
		}
		CPPUNIT_ASSERT_EQUAL_MESSAGE("myLogHandler default case", 0, 1);
		break;
	}
}

/*!
	\brief Test Unit for SLogMsgHandler
*/
class SLogHandlerTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(SLogHandlerTest);
	CPPUNIT_TEST(testRegisterOwnHandler);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp()
	{
	}

	void tearDown()
	{
		fflush(stdout);
	}

	void testRegisterOwnHandler()
	{
		sInstallCustomMsgLogHandler(myLogHandler);

		sDebug("debug");
		sWarning("warning");
		sCritical("critical");
		sFatal("fatal");
	}

};

#endif // SLOGHANDLER_TEST_H
