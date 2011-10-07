/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include <iostream>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/TestListener.h>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include "../src/core/sapplication.h"

/*
	Test Runner main function
*/
int main (int /*argc*/, char** /*argv*/)
{
	SApplication::init();

	SApplication::getInstance()->start();

	CppUnit::TextUi::TestRunner runner;
	CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

	CppUnit::TestResult resultController;
	CppUnit::BriefTestProgressListener progressListner;

	resultController.addListener(&progressListner);

	runner.addTest(registry.makeTest());
	runner.run(/*resultController*/);
	return 0;
}
