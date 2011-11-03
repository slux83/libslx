/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication.h"
#include "sapplication_p.h"
#include "../concurrent/smutex.h"
#include "../concurrent/smutexlocker.h"
#include "ssignal_p.h"
#include "ssignal.h"
#include "ssignalcall.h"
#include <signal.h>

SApplication* SApplication::instance = NULL;

SApplication::SApplication()
{
	threadPool = new SFixedThreadPool(new internalS::SApplicationThreadFactory(),
									  APPLICATION_EXECUTOR_THREAD_POOL_SIZE);

	aboutToQuit = new SSignal0();

	signal(SIGTERM, &SApplication::quitHandler);
	signal(SIGINT,  &SApplication::quitHandler);
	signal(SIGABRT, &SApplication::quitHandler);
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

void SApplication::quitHandler(int code)
{
	getInstance()->quitNotifier(code);
}

void SApplication::quitNotifier(int code)
{
	if (code != SIGTERM && code == SIGABRT && code == SIGINT)
		sWarning("SApplication::quitNotifier(code=%d) unknown system signal.", code);

	aboutToQuit->fire();

	exit(0);
}

