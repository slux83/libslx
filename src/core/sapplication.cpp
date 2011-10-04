/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication.h"
#include "../concurrent/smutex.h"
#include "../concurrent/smutexlocker.h"
#include "ssignal_p.h"

SApplication* SApplication::instance = NULL;

SApplication::SApplication()
{
}

void SApplication::init()
{
	if (instance != NULL)
		return;

	if (instance == NULL)
		instance = new SApplication();
}

SApplication* SApplication::getInstance()
{
	S_ASSERT_MSG(instance != NULL, "SApplication::getInstance() instance not initialized");

	return instance;
}

