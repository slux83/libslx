/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SAPPLICATION_H
#define SAPPLICATION_H

#include "../global/sglobal.h"
#include "../concurrent/smutex.h"
#include <map>

class SAbstractSignalSlotConnection;
class SSlot;

/*!
	\brief Global application instance
	This class must be used by user applications that want use this library.
	The init() function must be invoked in the main()
	The getInstance() function must be invoked to get the singletone instance

	\note all functions are thread-safe except init()
*/
class SApplication
{

protected:
	//! Singleton instance
	static SApplication *instance;

private:
	//Constructor
	explicit SApplication();

	//! Mutex for slots
	std::map<SSlot*, SMutex*> slotVsMutex;

	//! Internal use mutex
	SMutex ownLocker;

public:
	//! SApplication instance init. \reentrant This function isn't thread safe
	static void init();

	//! SApplication instance getter. \reentrant This function isn't thread safe
	static SApplication* getInstance();

	//! Get the unique mutex for the slotTarget instance
	SMutex* _getSlotMutex(SSlot *slotTarget);

	//! Register a new slot
	void _registerSlot(SSlot *slotTarget);

	//! Unregister a slot
	void _unregisterSlot(SSlot *slotTarget);

};

#endif // SAPPLICATION_H
