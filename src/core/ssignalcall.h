/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALCALL_H
#define SSIGNALCALL_H

#include "ssignal_p.h"

namespace internalS
{


	/*!
		\brief Signal call for async invocations
	 */
	class SSignalCall
	{

	private:
		internalS::SAbstractSignal *signal;

	public:

		explicit SSignalCall(SAbstractSignal *signalSource)
		{
			signal = signalSource;
		}
	};
}

#endif // SSIGNALCALL_H
