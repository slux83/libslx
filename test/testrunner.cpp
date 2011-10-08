/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include "cppunit/smutex_test.h"

/*
	Test Runner main function
*/
int main (int /*argc*/, char** /*argv*/)
{
	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

	CppUnit::TestResult resultController;
	CppUnit::BriefTestProgressListener progressListner;
	resultController.addListener(&progressListner);

	runner.addTest(registry.makeTest());
	runner.run(resultController);
	return 0;
}
