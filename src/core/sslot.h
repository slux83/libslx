/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSLOT_H
#define SSLOT_H

#include "ssignal_p.h"
#include <set>

/*!
	\brief Slot class.
	All user classes that have a slot must extends this class
*/
class SSlot
{
private:
	//! Signals connected to this object
	std::set<internalS::SAbstractSignal*> connectedSignals;

public:
	//! Mutex for slots invocations. It can be user by subclasses too
	SMutex slotCallbackLock;

    SSlot();

	virtual ~SSlot();

	void _addConnectedSignal(internalS::SAbstractSignal *signal);

	void _signalDestroyed(internalS::SAbstractSignal *signal);
};

#endif // SSLOT_H
