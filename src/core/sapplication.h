/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "../global/sglobal.h"
#include "../concurrent/sunboundedblockingqueue.h"
#include "../concurrent/sfixedthreadpool.h"
#include <map>

class SSignal0;
class SAbstractSignalSlotConnection;
class SSlot;
namespace internalS
{
	class SSignalCall;
	class SAsyncExecutorThread;

	//! \internal
	struct Shutdown
	{
		unsigned int size;
	};
}

/*!
	\brief Global application instance
	This class must be used by user applications that want use this library.
	The init() function must be invoked in the main()
	The getInstance() function must be invoked to get the singletone instance

	\note all functions are thread-safe except init()
*/
class SApplication
{

	friend class internalS::SAsyncExecutorThread;

protected:
	//! Singleton instance
	static SApplication *instance;

	//! Blocking queue for async signal invocations
	SUnboundedBlockingQueue<internalS::SSignalCall> signalAsyncCall;

	//! Executors thread pool
	SAbstractThreadPool *threadPool;

	//! Function that fires the signal aboutToQuit
	void quitNotifier(int code);

private:
	//! Constructor
	explicit SApplication();

	/*! Handler called on application exit (SIGTERM, SIGINT or SIGABRT).
		\param signal code
	*/
	static void quitHandler(int code);

public:
	//! SApplication instance init. \reentrant This function isn't thread safe
	static void init();

	//! SApplication instance getter. \reentrant This function isn't thread safe
	static SApplication* getInstance();

	/*! SApplication starter fucntion.
		This function starts the signal/slot thread pool for asynchronous connections
		\reentrant This function isn't thread safe
	*/
	void start();

	//! Enqueue call into the invocation queue
	void addAsyncCall(const internalS::SSignalCall &call);

	/*!
		This function should be called befor the real application shutdown.
		\note After this invocation, all asynchronous signals cannot be fired!
	*/
	void shutdown();

	/*! About to quit signal.
		\note this signal is synchronous which means that the application will exit after the notify.
	*/
	SSignal0 *aboutToQuit;
};

#endif // SAPPLICATION_H
