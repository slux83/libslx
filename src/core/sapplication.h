/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "../global/sglobal.h"
#include "../concurrent/sunboundedblockingqueue.h"
#include <map>

class SAbstractSignalSlotConnection;
class SSlot;
namespace internalS
{
	class SSignalCall;
}

/*!
	\brief Global application instance
	This class must be used by user applications that want use this library.
	The init() function must be invoked in the main()
	The getInstance() function must be invoked to get the singletone instance

	\note all functions are thread-safe except init()

	\todo we really need this shit?
*/
class SApplication
{

protected:
	//! Singleton instance
	static SApplication *instance;

	//! Blocking queue for async signal invocations
	SUnboundedBlockingQueue<internalS::SSignalCall> signalAsincCall;

private:
	//Constructor
	explicit SApplication();

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
};

#endif // SAPPLICATION_H
