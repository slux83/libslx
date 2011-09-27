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
	S_ASSERT_MSG(signal != NULL, "SSlot::_addConnectedSignal() argument NULL");
	connectedSignals.insert(signal);
}

void SSlot::_signalDestroyed(internalS::SAbstractSignal *signal)
{
	S_ASSERT_MSG(signal != NULL, "SSlot::_signalDestroyed() argument NULL");
	connectedSignals.erase(signal);
}
