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
	for (std::list<internalS::SAbstractSignal*>::const_iterator it = connectedSignals.begin();
		 it != connectedSignals.end(); it++)
	{
		internalS::SAbstractSignal *signal = *it;
		if (signal != NULL)
			signal->disconnectAll(this);
	}

	connectedSignals.clear();
}
void SSlot::_addConnectedSignal(internalS::SAbstractSignal *signal)
{
	S_ASSERT_MSG(signal != NULL, "SSlot::_addConnectedSignal() NULL argument");
	connectedSignals.push_back(signal);
}

void SSlot::_signalDestroyed(internalS::SAbstractSignal *signal)
{
	S_ASSERT_MSG(signal != NULL, "SSlot::_signalDestroyed() NULL argument");
	connectedSignals.remove(signal);
}
