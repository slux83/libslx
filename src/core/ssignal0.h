/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL0_H
#define SSIGNAL0_H

#include <list>
#include "ssignal_p.h"
#include "sslot.h"

class SSignal0 : public internalS::SAbstractSignal
{
	typedef std::list<internalS::SAbstractSignalSlotConnection0*> ConnectionList;
	typedef typename ConnectionList::iterator ConnectionListConstIterator;

private:
	ConnectionList connections;

public:

	virtual ~SSignal0()
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			//Notify to the slot that the signal is dying
			(*it)->getTarget()->_signalDestroyed(this);

			delete (*it);
			it++;
		}

		connections.clear();
	}

	template <class SSlotType>
	bool connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)())
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (dynamic_cast<SSlot*>(slotTarget) == NULL)
		{
			sWarning("SSignal0::connect() first argument (slotTarget) doesn't extends SSlot class.");
			return false;
		}

		internalS::SSignalSlotConnection0<SSlotType> *conn =
				new internalS::SSignalSlotConnection0<SSlotType> (slotTarget, callableMethod);
		connections.push_back(conn);
		dynamic_cast<SSlot*>(slotTarget)->_addConnectedSignal(this);

		return true;
	}

	void fire()
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (connections.empty())
			return;

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			(*it)->fire();
			it++;
		}
	}

	void operator()()
	{
		fire();
	}

	virtual void disconnectAll(SSlot *slotTarget)
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			if ((*it)->getTarget() == slotTarget)
				it = connections.erase(it);
			else
				it++;
		}
	}
};

#endif // SSIGNAL0_H
