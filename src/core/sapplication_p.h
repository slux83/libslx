/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SAPPLICATION_P_H
#define SAPPLICATION_P_H

#include "../concurrent/sabstractthreadpool.h"

/*!
	\namespace internal stuff
*/
namespace internalS
{
	class SApplicationThreadFactory	: public SAbstractThreadFactory
	{
	private:
		//! Thread creation counter
		unsigned int threadCounter;

	public:
		//! Constructor
		SApplicationThreadFactory();

		/*!
			Create a new thread instance.
			The new thread life cycle will be managed by the caller thread pool
		*/
		virtual SThread* createThreadInstance();
	};

	/*!
		\brief custom thread that executes asynchronous signals
	*/
	class SAsyncExecutorThread : public SThread
	{
	public:
		//! Constructor
		SAsyncExecutorThread(const std::string &threadName = "");

		//! Override
		virtual void run();
	};
}

#endif // SAPPLICATION_P_H
