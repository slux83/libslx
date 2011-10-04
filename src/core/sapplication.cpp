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

SMutex* SApplication::_getSlotMutex(SSlot *slotTarget)
{
	S_ASSERT_MSG(slotTarget != NULL, "SApplication::_getSlotMutex() slotTarget is NULL");

	SMutexLocker locker(&ownLocker);
	S_USE_VAR(locker);

	return slotVsMutex[slotTarget];
}


void SApplication::_registerSlot(SSlot *slotTarget)
{
	SMutexLocker locker(&ownLocker);
	S_USE_VAR(locker);

	S_ASSERT_MSG(slotVsMutex.find(slotTarget) == slotVsMutex.end(),
				 "SApplication::_registerSlot() slotTarget already registered");

	slotVsMutex[slotTarget] = new SMutex();
}


void SApplication::_unregisterSlot(SSlot *slotTarget)
{
	SMutexLocker locker(&ownLocker);
	S_USE_VAR(locker);

	S_ASSERT_MSG(slotVsMutex.find(slotTarget) != slotVsMutex.end(),
				 "SApplication::_unregisterSlot() unknown slotTarget");

	delete slotVsMutex[slotTarget];
	slotVsMutex[slotTarget] = NULL;

	slotVsMutex.erase(slotTarget);
}
