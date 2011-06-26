/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "smutex.h"
#include "../global/sglobal.h"

SMutex::SMutex(SMutex::RecursionMode recursionMode)
{
    pthread_mutexattr_t mutexAttr;

    if (recursionMode == SMutex::Recursive)
    {
        pthread_mutexattr_init(&mutexAttr);
        pthread_mutexattr_settype(&mutexAttr, PTHREAD_MUTEX_RECURSIVE);
    }

    pthread_mutex_init(&mutex, (recursionMode == SMutex::Recursive)? &mutexAttr : NULL);

    if (recursionMode == SMutex::Recursive)
    {
        //Destroy local attribute
        pthread_mutexattr_destroy(&mutexAttr);
    }
}

SMutex::~SMutex()
{
    pthread_mutex_destroy(&mutex);
}

void SMutex::lock()
{
    pthread_mutex_lock(&mutex);
}

void SMutex::unlock()
{
    pthread_mutex_unlock(&mutex);
}

bool SMutex::tryLock()
{
    int returnValue = pthread_mutex_trylock(&mutex);

    return (returnValue != EBUSY);
}
