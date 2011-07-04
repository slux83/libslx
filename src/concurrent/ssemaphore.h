/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSEMAPHORE_H
#define SSEMAPHORE_H

#include "../global/sglobal.h"

class SMutex;
class SWaitCondition;

/*!
    \brief The SSemaphore class represent a n-ari semaphore.
    It is implemented in the classic way using one SMutex and
    a SWaitCondition.
*/
class SSemaphore
{
private:
    //! Internal mutex
    SMutex *mutexWaiter;

    //! Internal wait condition
    SWaitCondition *waitCond;

    //! Available resources
    int available;

public:
    /*!
        Contructor
        \param size the number of resources (default 1)
    */
    SSemaphore(int n = 1);

    /*! Destructor
        \warning Destroying a semaphore that is in use may result in undefined behavior.
    */
    virtual ~SSemaphore();

    /*!
        Acquire n resources. If n > getAvailable(),
        this call will block until enough resources are available.
        \param n the numbers of resources to acquire (default 1)
    */
    void acquire(int n = 1);

    /*!
        \return the number of resources currently available to the semaphore.
        This number can never be negative.
    */
    int	getAvailable() const;

    /*!
        Release n resources.
        This function can be used to "create" resources as well. For example:
        \code
        SSemaphore sem(5);
        sem.acquire(5);
        sem.release(5);     //The semaphore now has 5 resources free
        sem.release(10);    //The semaphore now has 10 resources free (created)
        \endcode

        \param n number of resources to release
    */
    void release(int n = 1);

    /*!
        Tries to acquire \c n resources.
        \return true if getAvailable() < \c n this call immediately returns false and doesn't acquire resources

        \note This call never blocks the caller
    */
    bool tryAcquire(int n = 1);
};

#endif // SSEMAPHORE_H
