/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL0_H
#define SSIGNAL0_H

#include <list>
#include "ssignal_p.h"

class SSignal0
{
	typedef std::list<internalS::SAbstractSignalSlotConnection0*> ConnectionList;
	typedef typename ConnectionList::const_iterator ConnectionListConstIterator;

private:
	ConnectionList connections;

public:
	template <class SSlotType>
	void connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)())
	{
		internalS::SSignalSlotConnection0<SSlotType> *conn =
				new internalS::SSignalSlotConnection0<SSlotType> (slotTarget, callableMethod);
		connections.push_back(conn);
	}

	void fire()
	{
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
};

#endif // SSIGNAL0_H
