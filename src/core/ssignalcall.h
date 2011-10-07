/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALCALL_H
#define SSIGNALCALL_H

#include <vector>
#include "ssignal_p.h"
#include "svariant.h"

/*!
	\namespace internal usage
*/
namespace internalS
{
	/*!
		\brief Signal call for async invocations
	 */
	class SSignalCall
	{

	private:
		//! the signal to be invoked
		internalS::SAbstractSignal *signal;

		//! the arguments
		std::vector<SVariant> arguments;

	public:
		/*! Constructor.
			\param signalSource
		*/
		explicit SSignalCall(SAbstractSignal *signalSource)
		{
			S_ASSERT(signalSource != NULL);
			signal = signalSource;
		}

		/*!
			Add an argument (as SVariant)
			\param value - the value of the argument
			\param index - position of the argument in the invocation chain (starting from 0)
		*/
		template <typename T>
		void addArgument(T value, int index)
		{
			arguments[index] = SVariant(value);
		}

		/*!
			Get the value at position index
			\param index - argument position
			\return the SVariant representing the argument value
			\sa addArgument
		*/
		SVariant getArgument(unsigned int index) const
		{
			S_ASSERT_MSG(index >=0 && index < arguments.size(),
						 "SSignalCall::getArgument(int) index out of bound");

			return arguments[index];
		}

		//! Getter for signal
		internalS::SAbstractSignal* getSignalSource()
		{
			return signal;
		}
	};
}

#endif // SSIGNALCALL_H
