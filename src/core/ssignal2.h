/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL2_H
#define SSIGNAL2_H

#include <list>
#include "ssignal_p.h"

#include "sslot.h"
#include "ssignalcall.h"

template <typename arg1, typename arg2>
class SSignal2 : public internalS::SAbstractSignal
{
	typedef std::list< internalS::SAbstractSignalSlotConnection2<arg1, arg2> *> ConnectionList;
	typedef typename ConnectionList::iterator ConnectionListConstIterator;

private:
	ConnectionList connections;

public:

	explicit SSignal2(int signalFlags = SSignalFlagThreadSafe | SSignalFlagSyncConnection)
		: internalS::SAbstractSignal(signalFlags)
	{
	}

	virtual ~SSignal2()
	{
		clear();
	}

	virtual void clear()
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
	bool connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)(arg1, arg2))
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (dynamic_cast<SSlot*>(slotTarget) == NULL)
		{
			sWarning("SSignal2::connect() first argument (slotTarget) doesn't extends SSlot class.");
			return false;
		}

		internalS::SSignalSlotConnection2<SSlotType, arg1, arg2> *conn =
				new internalS::SSignalSlotConnection2<SSlotType, arg1, arg2> (slotTarget, callableMethod, flags);
		connections.push_back(conn);
		dynamic_cast<SSlot*>(slotTarget)->_addConnectedSignal(this);

		return true;
	}

	void fire(arg1 a1, arg2 a2)
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (connections.empty())
			return;

		//Async call?
		if (flags & SSignalFlagAsyncConnection)
		{
			internalS::SSignalCall sc(this);
			sc.addArgument(a1, 0);
			sc.addArgument(a2, 1);
			SApplication::getInstance()->addAsyncCall(sc);

			return;
		}

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			(*it)->fire(a1, a2);
			it++;
		}
	}

	void operator()(arg1 a1, arg2 a2)
	{
		fire(a1, a2);
	}

	virtual void disconnectAll(SSlot *slotTarget)
	{
		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			if ((*it)->getTarget() == slotTarget)
			{
				delete (*it);
				it = connections.erase(it);
			}
			else
				it++;
		}
	}

	virtual void asyncExec(const std::map<int, SVariant> &args)
	{
		S_ASSERT_MSG(args.size() == 2, "SSignal2::asyncExec(...) invalid argument size");

		SMutexLocker locker(&signalMutex); S_USE_VAR(locker);

		if (connections.empty())
			return;

		ConnectionListConstIterator it = connections.begin();
		ConnectionListConstIterator end = connections.end();

		while (it != end)
		{
			(*it)->fire(args.at(0), args.at(1));
			it++;
		}
	}
};

#endif // SSIGNAL2_H
