/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALCALL_H
#define SSIGNALCALL_H

#include <vector>
#include "ssignal_p.h"
#include "svariant.h"

namespace internalS
{
	/*!
		\brief Signal call for async invocations
	 */
	class SSignalCall
	{

	private:
		internalS::SAbstractSignal *signal;
		std::vector<SVariant> arguments;

	public:
		explicit SSignalCall(SAbstractSignal *signalSource)
		{
			signal = signalSource;
		}

		/*!
			Add an argument (as SVariant)
			\param value - the value of the argument
			\param index - position of the argument in the invocation chain
		*/
		template <typename T>
		void addArgument(T value, int index)
		{
			arguments[index] = SVariant(value);
		}
	};
}

#endif // SSIGNALCALL_H
