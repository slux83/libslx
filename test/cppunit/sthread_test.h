/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STHREAD_TEST_H
#define STHREAD_TEST_H

#include <unistd.h>
#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/sthread.h"

/*!
    \brief Test Unit for SMutex object
*/
class SThreadTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SThreadTest);
CPPUNIT_TEST(testSingleThreadJoin);
CPPUNIT_TEST(testMultipleThreadJoin);
CPPUNIT_TEST_SUITE_END();

private:
    //Internal test thread class
    class TestTherad : public SThread
    {
    public:
        TestTherad(const std::string &threadName) : SThread(threadName) {}

    protected:
        virtual void run()
        {
            SThread::run();

            for (int i=0; i<3; ++i)
            {
#if 0   /* Test debugging */
                std::cout << "Thread: '" << getName() << "'' i=" << i << std::endl;
#endif
                sleep(1);
            }
        }
    };

    TestTherad *thread1, *thread2, *thread3, *thread4;

public:
    void setUp()
    {
        thread1 = new TestTherad("Thread-1");
        thread2 = new TestTherad("Thread-2");
        thread3 = new TestTherad("Thread-3");
        thread4 = new TestTherad("Thread-4");
    }

    void tearDown()
    {
        delete thread1;
        delete thread2;
        delete thread3;
        delete thread4;

        thread1 = NULL;
        thread2 = NULL;
        thread3 = NULL;
        thread4 = NULL;

		fflush(stdout);
	}


    void testSingleThreadJoin()
    {
        thread1->start();

        CPPUNIT_ASSERT_EQUAL(true, thread1->isRunning());

        bool joinReturnValue = thread1->join();
        CPPUNIT_ASSERT_EQUAL(true, joinReturnValue);
        CPPUNIT_ASSERT_EQUAL(false, thread1->isRunning());
    }

    void testMultipleThreadJoin()
    {
        thread2->start();
        thread3->start();
        thread4->start();

        CPPUNIT_ASSERT_EQUAL(true, thread2->isRunning());
        CPPUNIT_ASSERT_EQUAL(true, thread3->isRunning());
        CPPUNIT_ASSERT_EQUAL(true, thread4->isRunning());

        bool joinReturnValue2 = thread2->join();
        bool joinReturnValue3 = thread3->join();
        bool joinReturnValue4 = thread4->join();

        CPPUNIT_ASSERT_EQUAL(true, joinReturnValue2);
        CPPUNIT_ASSERT_EQUAL(true, joinReturnValue3);
        CPPUNIT_ASSERT_EQUAL(true, joinReturnValue4);

        CPPUNIT_ASSERT_EQUAL(false, thread2->isRunning());
        CPPUNIT_ASSERT_EQUAL(false, thread3->isRunning());
        CPPUNIT_ASSERT_EQUAL(false, thread4->isRunning());
    }
};

#endif // STHREAD_TEST_H
