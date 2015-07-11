/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SBLOCKINGQUEUE_H
#define SBLOCKINGQUEUE_H

#include "smutex.h"
#include "swaitcondition.h"
#include "smutexlocker.h"
#include "sabstractblockingqueue.h"
#include <list>

/*!
	\brief Unbounded Blocking Queue thread safe.

	This class implements a thread safe and unbounded blocking queue. Tipically
	this container is very useful when one or more producer threads fill data
	into the queue. One consumer thread read the data from the queue using
	dequeue() that blocks the thread if no datas are available. The consumer(s)
	thread will be unlocked when some producer thread inserts some data into the
	queue.

	\note internally a std::list is used.

	\note you can use also many consumer threads and many producer threads.
*/
template <typename T> class SUnboundedBlockingQueue : public SAbstractBlockingQueue<T>
{
private:
	//! The internal container.
	std::list<T> queue;

	//! Locker for the waitBlockingQueueAccess (not recursive)
	mutable SMutex waitConditionLocker;

	//! The internal SWaitCondition
	SWaitCondition waitBlockingQueueAccess;

public:
	//! Empty constructor
	inline SUnboundedBlockingQueue()
	{
		waitConditionLocker = SMutex(SMutex::NonRecursive);
	}

	/*! Enqueue the passed element to the end of the queue
		\param T Element to push back
		\return always TRUE
	*/
	inline bool enqueue(const T &t)
	{
		SMutexLocker locker(&waitConditionLocker);
		S_USE_VAR(locker);

		queue.push_back(t);

		waitBlockingQueueAccess.wakeupOne();

		return true;
	}

	/*!
		\return true if the queue is empty, false otherwise
	*/
	inline bool isEmpty() const
	{
		SMutexLocker locker(&waitConditionLocker);
		S_USE_VAR(locker);

		return queue.empty();
	}

	/*! Dequeue the element in front of the queue if any.
		If the queue is empty, the caller thread will be blockd until datas
		became available.
		\return T element from queue's front
	*/
	inline T dequeue()
	{
		SMutexLocker locker(&waitConditionLocker);
		S_USE_VAR(locker);

		while (queue.empty())
		{
			waitBlockingQueueAccess.wait(&waitConditionLocker);
		}

		T returnVal = queue.front();
		queue.pop_front();

		return returnVal;
	}
};

#endif // SBLOCKINGQUEUE_H
