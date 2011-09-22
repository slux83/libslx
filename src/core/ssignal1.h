/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL1_H
#define SSIGNAL1_H

#include <list>
#include "ssignal_p.h"

template <typename arg1>
class SSignal1
{
	typedef std::list< internalS::SAbstractSignalSlotConnection1<arg1> *> ConnectionList;
	typedef typename ConnectionList::const_iterator ConnectionListConstIterator;

private:
	ConnectionList connections;

public:
	template <class SSlotType>
	void connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)(arg1))
	{
		internalS::SSignalSlotConnection1<SSlotType, arg1> *conn =
				new internalS::SSignalSlotConnection1<SSlotType, arg1> (slotTarget, callableMethod);
		connections.push_back(conn);
	}

	void fire(arg1 a1)
	{
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
};

#endif // SSIGNAL1_H
