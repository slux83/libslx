/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "smutex.h"
#include "../global/sglobal.h"

SMutex::SMutex(RecursionMode recursionMode)
{
    pthread_mutexattr_t *mutexAttr = NULL;

    if (recursionMode == Recursive)
    {
        pthread_mutexattr_init(mutexAttr);
        pthread_mutexattr_settype(mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    }

    pthread_mutex_init(mutex, mutexAttr);

    if (mutexAttr)
    {
        //Destroy local attribute
        pthread_mutexattr_destroy(mutexAttr);
        mutexAttr = NULL;
    }

    S_ASSERT(mutex != NULL);
}

SMutex::~SMutex()
{
    if (mutex)
    {
        pthread_mutex_destroy(mutex);
        mutex = NULL;
    }
}

void SMutex::lock()
{
    pthread_mutex_lock(mutex);
}

void SMutex::unlock()
{
    pthread_mutex_unlock(mutex);
}

bool SMutex::tryLock()
{
    int returnValue = pthread_mutex_trylock(mutex);

    return (returnValue != EBUSY);
}
