/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSEMAPHORE_TEST_H
#define SSEMAPHORE_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/swaitcondition.h"
#include "../../src/concurrent/smutex.h"
#include "../../src/concurrent/sthread.h"
#include "../../src/concurrent/ssemaphore.h"

/*!
    \brief Test Unit for SSemaphore object
*/
class SSemaphoreTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SSemaphoreTest);
CPPUNIT_TEST(testAcquireReleaseBinarySemaphore);
CPPUNIT_TEST(testAcquireReleaseTryAcquireNArySemaphore);
CPPUNIT_TEST_SUITE_END();

private:
    // Global resources to be test
    static SSemaphore *semaphore1;

    //Internal test thread class -------------------------------------- [BEGIN]
    class TestTherad : public SThread
    {
    private:
        // Resources of semaphore1 to release
        int toRelease;

    public:
        TestTherad(const std::string &threadName, int n) :
            SThread(threadName)
        {
            toRelease = n;
            S_ASSERT(toRelease >= 0);
        }

    protected:
        virtual void run()
        {
            SThread::run();

            sleep(1);   //wait some time...

            //Release the semaphore1
            semaphore1->release(toRelease);

        }
    };
    //Internal test thread class ---------------------------------------- [END]

    TestTherad *t1;

public:
    void setUp()
    {
        semaphore1 = NULL;
        t1 = NULL;
    }

    void tearDown()
    {
        if (semaphore1 != NULL)
        {
            delete semaphore1;
            semaphore1 = NULL;
        }

        if (t1 != NULL)
        {
            delete t1;
            t1 = NULL;
        }

		fflush(stdout);
    }

    void testAcquireReleaseBinarySemaphore()
    {
        t1 = new TestTherad("Thread-1", 1);
        semaphore1 = new SSemaphore();  //available = 1 (default)

        sleep(1);   //wait until the thread is started

        //Now we have 1 resource free
        CPPUNIT_ASSERT_EQUAL(1, semaphore1->getAvailable());

        //Acquire 1 resource
        semaphore1->acquire();

        //Now we have no resources free
        CPPUNIT_ASSERT_EQUAL(0, semaphore1->getAvailable());

        //Let the thread t1 release 1 resource while this main thread acquires another resource
        t1->start();
        semaphore1->acquire();  //Blocks until the t1 release the resource
        t1->join();

        //Now we have no resources free
        CPPUNIT_ASSERT_EQUAL(0, semaphore1->getAvailable());

        semaphore1->release();  //restore all resources

        CPPUNIT_ASSERT_EQUAL(1, semaphore1->getAvailable());

        delete t1;
        t1 = NULL;

        delete semaphore1;
        semaphore1 = NULL;
    }

    void testAcquireReleaseTryAcquireNArySemaphore()
    {
        t1 = new TestTherad("Thread-1", 3);
        semaphore1 = new SSemaphore(3);

        sleep(1);  //wait until the thread is started;

        //Now we have 3 resources free
        CPPUNIT_ASSERT_EQUAL(3, semaphore1->getAvailable());

        //Cannot acquire 10 resources
        CPPUNIT_ASSERT_EQUAL(false, semaphore1->tryAcquire(10));

        //Cannot acquire 10 resources
        CPPUNIT_ASSERT_EQUAL(true, semaphore1->tryAcquire(3));

        t1->start();
        semaphore1->acquire(3); //Blocksuntil the t1 release the resources
        t1->join();

        //Now we have no resources free
        CPPUNIT_ASSERT_EQUAL(0, semaphore1->getAvailable());

        semaphore1->release(10);  //restore all resources and add others

        //Now we have ten resources free
        CPPUNIT_ASSERT_EQUAL(10, semaphore1->getAvailable());

        //acquire all 10 resources
        semaphore1->acquire(10);

        //Now we have no resources free
        CPPUNIT_ASSERT_EQUAL(0, semaphore1->getAvailable());

        //Restore the initial three resources
        semaphore1->release(3);

        //Now we have 3 resources free
        CPPUNIT_ASSERT_EQUAL(3, semaphore1->getAvailable());

        delete t1;
        t1 = NULL;

        delete semaphore1;
        semaphore1 = NULL;

    }
};

#endif // SSEMAPHORE_TEST_H
