/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SABSTRACTBLOCKINGQUEUE_H
#define SABSTRACTBLOCKINGQUEUE_H

/*!
	\brief Abstract base class for blocking queues.

	Implementations: SUnboundedBlockingQueue
*/
template<typename T> class SAbstractBlockingQueue
{
public:
	//! Destructor
	virtual ~SAbstractBlockingQueue() {}

	/*! Enqueue the passed element to the end of the queue
		\param T Element to push back
		\return the esite of this operation (some implementation - for example fixed queue - could return false)
	*/
	virtual bool enqueue(const T &t) = 0;

	/*!
		\return true if the queue is empty, false otherwise
	*/
	virtual bool isEmpty() const = 0;

	/*! Dequeue the element in front of the queue if any.
		If the queue is empty, the caller thread will be blockd until datas
		became available.
		\return T element from queue's front
	*/
	virtual T dequeue() = 0;
};

#endif // SABSTRACTBLOCKINGQUEUE_H
