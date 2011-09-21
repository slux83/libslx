/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL2_H
#define SSIGNAL2_H

#include <list>
#include "ssignal_p.h"

template <typename arg1, typename arg2>
class SSignal2
{
	typedef std::list< internalS::SAbstractSignalSlotConnection2<arg1, arg2> *> SignalList;
	typedef typename SignalList::const_iterator SignalListConstIterator;

private:
	SignalList connections;

public:
	template <class SSlotType>
	void connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)(arg1, arg2))
	{
		internalS::SSignalSlotConnection2<SSlotType, arg1, arg2> *conn =
				new internalS::SSignalSlotConnection2<SSlotType, arg1, arg2> (slotTarget, callableMethod);
		connections.push_back(conn);
	}

	void fire(arg1 a1, arg2 a2)
	{
		 SignalListConstIterator it = connections.begin();
		 SignalListConstIterator end = connections.end();

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
};

#endif // SSIGNAL2_H
