/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication_p.h"
#include "sapplication.h"
#include "ssignalcall.h"
#include <iostream>
#include <cstdio>

namespace internalS
{
	SApplicationThreadFactory::SApplicationThreadFactory()
	{
		threadCounter = 0;
	}

	SThread* SApplicationThreadFactory::createThreadInstance()
	{
		std::string threadName = "SApplicationThread-";
		char threadCounterStr[16];
		snprintf(threadCounterStr, 16, "%d", threadCounter);
		threadName += threadCounterStr;
		threadCounter++;

		SThread *thread = new SAsyncExecutorThread(threadName);

		return thread;
	}

	SAsyncExecutorThread::SAsyncExecutorThread(const std::string &threadName)
		: SThread(threadName)
	{

	}

	void SAsyncExecutorThread::run()
	{
		SApplication *app = SApplication::getInstance()->getInstance();
		S_ASSERT_MSG(app != NULL, "SAsyncExecutorThread::run() SApplication instance is NULL");

		S_FOREVER
		{
			internalS::SSignalCall call = app->signalAsyncCall.dequeue();
			call.invoke();
		}
	}
}
