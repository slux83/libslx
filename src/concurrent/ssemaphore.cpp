/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "ssemaphore.h"
#include "smutex.h"
#include "swaitcondition.h"
#include "smutexlocker.h"

SSemaphore::SSemaphore(int n)
{
    available = n;
    S_ASSERT(available >= 0);

    mutexWaiter = new SMutex(SMutex::NonRecursive);
    waitCond = new SWaitCondition();

}

SSemaphore::~SSemaphore()
{
    delete mutexWaiter;
    delete waitCond;

    waitCond = NULL;
    mutexWaiter = NULL;
}

void SSemaphore::acquire(int n)
{
    S_ASSERT(n >= 0);

    SMutexLocker locker(mutexWaiter);
    S_USE_VAR(locker);

    while (n > available)
        waitCond->wait(mutexWaiter);

    available -= n;
}

int	SSemaphore::getAvailable() const
{
    SMutexLocker locker(mutexWaiter);
    S_USE_VAR(locker);

    return available;
}

void SSemaphore::release(int n)
{
    S_ASSERT(n >= 0);

    SMutexLocker locker(mutexWaiter);
    S_USE_VAR(locker);

    available += n;

    waitCond->wakeupAll();
}

bool SSemaphore::tryAcquire(int n)
{
    S_ASSERT(n > 0);

    SMutexLocker locker(mutexWaiter);
    S_USE_VAR(locker);

    if (available < n)
        return false;

    available -= n;

    return true;
}
