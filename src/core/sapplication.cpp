/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication.h"
#include "../concurrent/smutex.h"
#include "../concurrent/smutexlocker.h"
#include "ssignal_p.h"
#include "ssignalcall.h"

SApplication* SApplication::instance = NULL;

SApplication::SApplication()
{
}

void SApplication::init()
{
	S_ASSERT_MSG(instance == NULL, "SApplication::getInstance() instance already initialized");

	if (instance == NULL)
		instance = new SApplication();
}

SApplication* SApplication::getInstance()
{
	S_ASSERT_MSG(instance != NULL, "SApplication::getInstance() instance not initialized");

	return instance;
}

void SApplication::start()
{
	//TODO
}

void SApplication::addAsyncCall(const internalS::SSignalCall &call)
{
	signalAsincCall.enqueue(call);
}
