/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sslot.h"

SSlot::SSlot()
{
}

SSlot::~SSlot()
{
	for (std::set<internalS::SAbstractSignal*>::const_iterator it = connectedSignals.begin();
		 it != connectedSignals.end(); it++)
	{
		(*it)->disconnectAll(this);
	}
	connectedSignals.clear();
}

void SSlot::_addConnectedSignal(internalS::SAbstractSignal *signal)
{
	connectedSignals.insert(signal);
}
