/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SABSTRACTTHREADPOOL_H
#define SABSTRACTTHREADPOOL_H

#include "../global/sglobal.h"
#include "../concurrent/sthread.h"

/*!
	\brief Abstract thread factory.
	User must implement this factory for using the Thread pools
*/
class SAbstractThreadFactory
{
public:
	//! Distructor
	virtual ~SAbstractThreadFactory() {}

	/*!
		Create a new thread instance.
		The new thread life cycle will be managed by the caller thread pool
	*/
	virtual SThread* createThreadInstance() = 0;
};

/*!
	\brief Abstract thread pool
*/
class SAbstractThreadPool
{
private:
	//! Private default constructor
	SAbstractThreadPool() {}

	//! Private copy constructor
	SAbstractThreadPool(const SAbstractThreadPool &right)
	{
		S_USE_VAR(right);
	}

protected:
	SAbstractThreadFactory *factory;

public:
	//! Distructor
	virtual ~SAbstractThreadPool()
	{
		if (factory != NULL)
		{
			delete factory;
			factory = NULL;
		}
	}

	SAbstractThreadPool(SAbstractThreadFactory *threadFactory)
	{
		S_ASSERT_MSG(threadFactory != NULL, "SAbstractThreadPool::SAbstractThreadPool() threadFactory is NULL");
		factory = threadFactory;
	}

	/*!
		Starts the thread pool.
		\param minPoolSize is the minimum size of the thread pool in terms of number of threads
		\param maxPoolSize is the maximum size of the thread pool in terms of number of threads

		Some implementations could ignore one or both of the arguments.
	*/
	virtual void start(int minPoolSize = -1, int maxPoolSize = -1) = 0;
};

#endif // SABSTRACTTHREADPOOL_H
