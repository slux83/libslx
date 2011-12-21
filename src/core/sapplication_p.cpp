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

			//Check if this thread is candidated to die during the shutdown
			if (call.getArgumentsSize() == 1 && call.getSignalSource() == NULL)
			{
				SVariant arg = call.getArgument(0);
				if (arg.isTypeOf<internalS::Shutdown>())
				{
					internalS::Shutdown s = arg;
					if (s.size > 0)
					{
						s.size -= 1;
						call.addArgument(s, 0);
						app->signalAsyncCall.enqueue(call);	//For the other threads
					}

					break;	//Exit
				}
			}

			call.invoke();
		}

		sDebug("SAsyncExecutorThread::run() [%s] exiting...", name.c_str());
	}
}
