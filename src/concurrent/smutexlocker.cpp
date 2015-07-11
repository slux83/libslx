/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "smutexlocker.h"
#include "smutex.h"
#include "../global/sglobal.h"

SMutexLocker::SMutexLocker(SMutex *mutex)
{
	this->mutex = NULL;
	alreadyUnlocked = false;

	if (mutex)
	{
		this->mutex = mutex;
		this->alreadyUnlocked = false;

		this->mutex->lock();
	}
}

SMutexLocker::~SMutexLocker()
{
	if (mutex)
	{
		if (!alreadyUnlocked)
			mutex->unlock();
	}
}

void SMutexLocker::unlock()
{
	if (mutex)
	{
		if (!alreadyUnlocked)
			mutex->unlock();
	}
}
