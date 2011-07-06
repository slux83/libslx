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
	alreadyUnlocked  = false;

    this->mutex->lock();
}

SMutexLocker::~SMutexLocker()
{
    S_ASSERT(mutex != NULL);

	if (!alreadyUnlocked)
		mutex->unlock();
}

void SMutexLocker::unlock()
{
	S_ASSERT(mutex != NULL);

	if (!alreadyUnlocked)
		mutex->unlock();
}
