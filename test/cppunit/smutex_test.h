/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SMUTEX_TEST_H
#define SMUTEX_TEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "../../src/concurrent/smutex.h"

/*!
    \brief Test Unit for SMutex object
*/
class SMutexTest : public CppUnit::TestFixture
{
CPPUNIT_TEST_SUITE(SMutexTest);

CPPUNIT_TEST(testLockUnlock);
CPPUNIT_TEST(testLockUnlockRecursive);
CPPUNIT_TEST(testLockUnlockNonRecursive);
CPPUNIT_TEST(testLockTryLockUnlockRecursiveNonRecursive);
CPPUNIT_TEST_SUITE_END();

private:
    SMutex *mutex;
    SMutex *mutexNonRecursive;

public:
    void setUp()
    {
        mutex = new SMutex(SMutex::Recursive);
        mutexNonRecursive = new SMutex(SMutex::NonRecursive);
    }

    void tearDown()
    {
        delete mutex;
        mutex = NULL;

        delete mutexNonRecursive;
        mutexNonRecursive = NULL;
    }

    void testLockUnlock()
    {
        mutex->lock();
        mutex->unlock();

    }

    void testLockUnlockRecursive()
    {
        /* Recursive lock */
        mutex->lock();
        mutex->lock();
        mutex->lock();
        CPPUNIT_ASSERT(mutexNonRecursive->tryLock());
        mutex->unlock();
    }

    void testLockUnlockNonRecursive()
    {
        mutexNonRecursive->lock();
        mutexNonRecursive->unlock();

    }

    void testLockTryLockUnlockRecursiveNonRecursive()
    {
        /* NON Recursive lock */
        mutexNonRecursive->lock();
        CPPUNIT_ASSERT(!mutexNonRecursive->tryLock());
        mutexNonRecursive->unlock();
    }
};

#endif // SMUTEX_TEST_H
