/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sfixedthreadpool.h"

SFixedThreadPool::SFixedThreadPool(SAbstractThreadFactory *threadFactory, unsigned char poolSize)
	: SAbstractThreadPool(threadFactory)
{
	S_ASSERT_MSG(poolSize > 0, "SFixedThreadPool::SFixedThreadPool() poolSize is ZERO");
	size = poolSize;
}

void SFixedThreadPool::start(int minPoolSize, int maxPoolSize)
{
	S_USE_VAR(minPoolSize);
	S_USE_VAR(maxPoolSize);

	//create the threads
	for (unsigned int i=0; i<size; ++i)
	{
		SThread *thread = factory->createThreadInstance();
		thread->start();
		threads.push_back(thread);
	}
}

SFixedThreadPool::~SFixedThreadPool()
{
	for (std::vector<SThread*>::iterator it = threads.begin(); it != threads.end();)
	{
		delete *it;
		it++;
	}

	threads.clear();
}

void SFixedThreadPool::setPoolSize(unsigned char newPoolSize)
{
	S_ASSERT_MSG(newPoolSize > 0, "SFixedThreadPool::setPoolSize() newPoolSize is ZERO");
	size = newPoolSize;
}

unsigned short SFixedThreadPool::getNumberOfThreads()
{
	return this->size;
}

void SFixedThreadPool::join()
{
	for (std::vector<SThread*>::iterator it = threads.begin(); it != threads.end();)
	{
		if (! (*it)->join())
			sWarning("SFixedThreadPool::join() [thread: '%s'] join failed", (*it)->getName().c_str());
		it++;
	}
}
