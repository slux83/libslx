/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication.h"
#include "sapplication_p.h"
#include "../concurrent/smutex.h"
#include "../concurrent/smutexlocker.h"
#include "ssignal_p.h"
#include "ssignalcall.h"

SApplication* SApplication::instance = NULL;

SApplication::SApplication()
{
	threadPool = new SFixedThreadPool(new internalS::SApplicationThreadFactory(),
									  APPLICATION_EXECUTOR_THREAD_POOL_SIZE);
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
	threadPool->start();
}

void SApplication::addAsyncCall(const internalS::SSignalCall &call)
{
	signalAsyncCall.enqueue(call);
}

internalS::SSignalCall SApplication::takeAsyncCall()
{
	return signalAsyncCall.dequeue();
}
