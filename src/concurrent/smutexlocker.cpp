/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "smutexlocker.h"
#include "smutex.h"
#include "../global/sglobal.h"

SMutexLocker::SMutexLocker(SMutex *mutex)
{
    S_ASSERT(mutex != NULL);

    this->mutex = mutex;

    this->mutex->lock();
}

SMutexLocker::~SMutexLocker()
{
    S_ASSERT(mutex != NULL);

    mutex->unlock();
}
