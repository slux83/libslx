/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SWAITCONDITION_TEST_H
#define SWAITCONDITION_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/swaitcondition.h"
#include "../../src/concurrent/smutex.h"
#include "../../src/concurrent/sthread.h"

/*!
    \brief Test Unit for SWaitCondition object
*/
class SWaitConditionTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SWaitConditionTest);
CPPUNIT_TEST(testWaitConditionWakeupOne);
CPPUNIT_TEST(testWaitConditionWakeupAll);
CPPUNIT_TEST_SUITE_END();

private:
    static SWaitCondition *cond1;

    //Internal test thread class -------------------------------------- [BEGIN]
    class TestTherad : public SThread
    {
    private:
        // Its important that the mutex used in the wait condition is non recursive!
        SMutex mutex;

    public:
        int numberOfWakeups;

        TestTherad(const std::string &threadName) : SThread(threadName)
        {
            mutex = SMutex(SMutex::NonRecursive);
            numberOfWakeups = 0;
        }


    protected:
        virtual void run()
        {
            SThread::run();

            //Waits 3 times that a second thread wakeup me
            while (numberOfWakeups < 3)
            {
                mutex.lock();

                bool esite = SWaitConditionTest::cond1->wait(&mutex);
                CPPUNIT_ASSERT_EQUAL(true, esite);
                numberOfWakeups++;
#if 0
                std::cout << "numberOfWakeups=" << numberOfWakeups << std::endl;
#endif

                mutex.unlock();
            }
        }
    };
    //Internal test thread class ---------------------------------------- [END]

    TestTherad *t1;

public:
    void setUp()
    {
        cond1 = new SWaitCondition();
        t1 = new TestTherad("Thread-1");
    }

    void tearDown()
    {
        delete cond1;
        cond1 = NULL;

        delete t1;
        t1 = NULL;
    }

    void testWaitConditionWakeupOne()
    {
        t1->start();

        sleep(2);   //wait until the thread is started

        //Now t1 waits for 5 wakeups
        CPPUNIT_ASSERT_EQUAL(0, t1->numberOfWakeups);

        //First wakeup
        cond1->wakeupOne();
        sleep(1);
        CPPUNIT_ASSERT_EQUAL(1, t1->numberOfWakeups);

        //Second wakeup
        cond1->wakeupOne();
        sleep(1);
        CPPUNIT_ASSERT_EQUAL(2, t1->numberOfWakeups);

        //Third wakeup
        cond1->wakeupOne();
        sleep(1);
        CPPUNIT_ASSERT_EQUAL(3, t1->numberOfWakeups);

        t1->join();
    }

    void testWaitConditionWakeupAll()
    {
        //TODO
    }
};

#endif // SWAITCONDITION_TEST_H
