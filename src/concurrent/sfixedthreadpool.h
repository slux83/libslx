/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SFIXEDTHREADPOOL_H
#define SFIXEDTHREADPOOL_H

#include "sabstractthreadpool.h"
#include "sthread.h"
#include <vector>

/*!
	\brief A Fixed size thread pool implementation
*/
class SFixedThreadPool : public SAbstractThreadPool
{
private:
	//! The thread pool size
	unsigned char size;

	//! The threads
	std::vector<SThread*> threads;

public:
	/*! Constructor
		\param threadFactory the thread factory implementation
		\param poolSize the size of the thread pool (must be greater than zero)
	*/
	explicit SFixedThreadPool(SAbstractThreadFactory *threadFactory, unsigned char poolSize);

	/*!
		Destructor
	*/
	virtual ~SFixedThreadPool();

	//! Start the thread pool. Arguments are ignored.
	virtual void start(int minPoolSize = -1, int maxPoolSize = -1);

	//! \return get the number of active threads
	virtual unsigned short getNumberOfThreads();

	/*! Join the ending of all threads */
	virtual void join();

	//! Setter for size. This has effects only befor the invocation of start()
	void setPoolSize(unsigned char newPoolSize);

};

#endif // SFIXEDTHREADPOOL_H
