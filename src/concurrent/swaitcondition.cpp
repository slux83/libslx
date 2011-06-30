/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "swaitcondition.h"
#include "smutex.h"

SWaitCondition::SWaitCondition() :
    waiters(0), wakeups(0)
{
    int retVal;

    if ((retVal = pthread_mutex_init(&mutex, NULL)) != 0)
        sWarning("SWaitCondition::SWaitCondition() failed to init mutex. error=%d", retVal);

    if ((retVal = pthread_cond_init(&cond, NULL)) != 0)
        sWarning("SWaitCondition::SWaitCondition() failed to init cond. error=%d", retVal);

}

SWaitCondition::~SWaitCondition()
{
    int retVal;

    if ((retVal = pthread_mutex_destroy(&mutex)) != 0)
        sWarning("SWaitCondition::~SWaitCondition() failed to destroy mutex. error=%d", retVal);

    if ((retVal = pthread_cond_destroy(&cond)) != 0)
        sWarning("SWaitCondition::~SWaitCondition() failed to destroy cond. error=%d", retVal);
}

bool SWaitCondition::wait(SMutex *mutex)
{
    if (!mutex)
        return false;

    if (mutex->getRecursiveMode() == SMutex::Recursive)
    {
        sWarning("SWaitCondition::wait(SMutex) Cannot wait on a recursive mutex");
        return false;
    }

    waiters++;
    mutex->unlock();

    int retCode;

    while (true)
    {
        retCode = pthread_cond_wait(&cond, &(this->mutex));

        //Sometimes some implementation of pthread lib can raise not valid wakeups
        if (wakeups == 0 && retCode == 0)
            continue;

        break;
    }

    S_ASSERT(waiters > 0);
    waiters--;

    if (retCode == 0)
    {
        S_ASSERT(wakeups > 0);
        wakeups--;
    }

    mutex->lock();

    return (retCode == 0);
}

void SWaitCondition::wakeupOne()
{
    int retVal;

    if ((retVal = pthread_mutex_lock(&mutex)) != 0)
        sWarning("SWaitCondition::wakeupOne() failed to lock mutex. error=%d", retVal);

    wakeups = sMin(wakeups + 1, waiters);

    if ((retVal = pthread_cond_signal(&cond)) != 0)
        sWarning("SWaitCondition::wakeupOne() failed to signal cond. error=%d", retVal);

    if ((retVal = pthread_mutex_unlock(&mutex)) != 0)
        sWarning("SWaitCondition::wakeupOne() failed to unlock mutex. error=%d", retVal);

}

void SWaitCondition::wakeupAll()
{
    int retVal;

    if ((retVal = pthread_mutex_lock(&mutex)) != 0)
        sWarning("SWaitCondition::wakeupAll() failed to lock mutex. error=%d", retVal);

    wakeups = waiters;

    if ((retVal = pthread_cond_broadcast(&cond)) != 0)
        sWarning("SWaitCondition::wakeupAll() failed to broadcast cond. error=%d", retVal);

    if ((retVal = pthread_mutex_unlock(&mutex)) != 0)
        sWarning("SWaitCondition::wakeupAll() failed to unlock mutex. error=%d", retVal);
}
