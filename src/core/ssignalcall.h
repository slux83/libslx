/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNALCALL_H
#define SSIGNALCALL_H

#include <map>
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
		std::map<int, SVariant> arguments;

	public:
		/*! Constructor.
			\param signalSource
		*/
		SSignalCall(SAbstractSignal *signalSource)
		{
			S_ASSERT(signalSource != NULL);
			signal = signalSource;
		}

		//! Copy constructor
		SSignalCall(const SSignalCall &right)
		{
			signal = right.signal;
			arguments = right.arguments;
		}

		SSignalCall()
		{
			signal = NULL;
		}

		/*!
			Add an argument (as SVariant)
			\param value - the value of the argument
			\param index - position of the argument in the invocation chain (starting from 0)
		*/
		template <typename T>
		void addArgument(T value, int index)
		{
			arguments.insert(std::make_pair(index, SVariant(value)));
		}

		/*!
			\overload
		*/
		void addArgument(const SVariant &value, int index)
		{
			arguments.insert(std::make_pair(index, value));
		}

		/*!
			Get the value at position index
			\param index - argument position
			\return the SVariant representing the argument value
			\sa addArgument
		*/
		SVariant getArgument(int index) const
		{
			S_ASSERT_MSG(arguments.find(index) != arguments.end(),
						 "SSignalCall::getArgument(int) index not found");

			return arguments.at(index);
		}

		//! Getter for signal
		internalS::SAbstractSignal* getSignalSource()
		{
			return signal;
		}

		//! Assignment operator
		SSignalCall& operator= (const SSignalCall &right)
		{
			if (this == &right)
				return *this;

			signal = right.signal;
			arguments = right.arguments;

			return *this;
		}

		//! Execute this call
		void invoke()
		{
			S_ASSERT_MSG(signal != NULL, "SSignalCall::invoke() NULL signal");
			signal->asyncExec(arguments);
		}

		int getArgumentsSize()
		{
			return arguments.size();
		}
	};
}

#endif // SSIGNALCALL_H
