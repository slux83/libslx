/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL1_H
#define SSIGNAL1_H

#include <list>
#include "ssignal_p.h"
#include "sslot.h"

template <typename arg1>
class SSignal1 : public internalS::SAbstractSignal
{
	typedef std::list< internalS::SAbstractSignalSlotConnection1<arg1> *> ConnectionList;
	typedef typename ConnectionList::iterator ConnectionListConstIterator;

private:
	ConnectionList connections;

public:
	virtual ~SSignal1()
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
	bool connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)(arg1))
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (dynamic_cast<SSlot*>(slotTarget) == NULL)
		{
			sWarning("SSignal1::connect() first argument (slotTarget) doesn't extends SSlot class.");
			return false;
		}

		internalS::SSignalSlotConnection1<SSlotType, arg1> *conn =
				new internalS::SSignalSlotConnection1<SSlotType, arg1> (slotTarget, callableMethod);
		connections.push_back(conn);
		dynamic_cast<SSlot*>(slotTarget)->_addConnectedSignal(this);

		return true;
	}

	void fire(arg1 a1)
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			(*it)->fire(a1);
			it++;
		}
	}

	void operator()(arg1 a1)
	{
		fire(a1);
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

#endif // SSIGNAL1_H
