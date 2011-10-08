/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sapplication_p.h"
#include "sapplication.h"
#include "ssignalcall.h"

namespace internalS
{
	SApplicationThreadFactory::SApplicationThreadFactory()
	{
		threadCounter = 0;
	}

	SThread* SApplicationThreadFactory::createThreadInstance()
	{
		std::string threadName = "SApplicationThread-";
		threadName += threadCounter++;
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
			internalS::SSignalCall call = app->takeAsyncCall();
			call.invoke();
		}
	}
}
