/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SBLOCKINGQUEUE_H
#define SBLOCKINGQUEUE_H

#include "smutex.h"
#include "swaitcondition.h"
#include "smutexlocker.h"
#include <list>

/*!
	\brief Unbounded blocking queue thread safe.
	This class implements a thread safe and unbounded blocking queue. Tipically
	this container is very useful when one or more producer threads fill data
	into the queue. One consumer thread read the data from the queue using
	dequeue() that blocks the thread if no datas are available. The consumer
	thread will be unlocked when some producer thread inserts some data into the
	queue.

	\note internally a std::list is used.

	\warning	the function dequeue() is reentrant. This means that you cannot
				use more than one consumer thread.
*/
template <typename T> class SBlockingQueue
{
private:
	//! The internal container.
	std::list<T> queue;

	//! Locker for internal operations
	mutable SMutex enqueueSerialAccess;

	//! Locker for the waitBlockingQueueAccess (not recursive)
	SMutex dequeueWaitConditionLocker;

	//! The internal SWaitCondition
	SWaitCondition waitBlockingQueueAccess;

public:
	//! Empty constructor
	inline SBlockingQueue()
	{
		dequeueWaitConditionLocker = SMutex(SMutex::NonRecursive);
	}

	/*! Enqueue the passed element to the end of the queue
		\param T Element to push back
	*/
	inline void enqueue(const T &t)
	{
		SMutexLocker locker(&enqueueSerialAccess);
		S_USE_VAR(locker);

		queue.push_back(t);

		waitBlockingQueueAccess.wakeupOne();
	}

	inline bool isEmpty() const
	{
		SMutexLocker locker(&enqueueSerialAccess);
		S_USE_VAR(locker);

		return queue.empty();
	}

	/*! Dequeue the element in front of the queue if any.
		If the queue is empty, the caller thread will be blockd until datas
		became available.
		\return T element from queue's front
		\warning If you use more than one thread to consume the queue's data, the behaviour is unpredictable.

	*/
	inline T dequeue()
	{
		dequeueWaitConditionLocker.lock();

		enqueueSerialAccess.lock();
		if (queue.empty())	//Protect the queue
		{
			enqueueSerialAccess.unlock();
			waitBlockingQueueAccess.wait(&dequeueWaitConditionLocker);
		}
		else
		{
			enqueueSerialAccess.unlock();
		}

		enqueueSerialAccess.lock();
		T returnVal = queue.front();
		queue.pop_front();
		enqueueSerialAccess.unlock();

		dequeueWaitConditionLocker.unlock();

		return returnVal;
	}
};

#endif // SBLOCKINGQUEUE_H
