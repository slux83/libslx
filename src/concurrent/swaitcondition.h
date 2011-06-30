/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SWAITCONDITION_H
#define SWAITCONDITION_H

#include <pthread.h>
#include "../global/sglobal.h"

class SMutex;

class SWaitCondition
{
private:
    //! Internal pthread mutex
    pthread_mutex_t mutex;

    //! Internal pthread condition var
    pthread_cond_t cond;

    //! Number of waiters
    int waiters;

    //! Number of thread to wakeup
    int wakeups;

public:
    //! Constructor
    SWaitCondition();

    //! Destructor
    virtual ~SWaitCondition();

    /*! Releases the locked mutex and waits on the wait condition.
        The mutex must be initially locked by the calling thread.
        If mutex is not in a locked state, this function returns immediately false.
        If mutex is a recursive mutex, this function returns immediately false.
        The mutex will be unlocked, and the calling thread will
        block until wakeupOne() or wakeupAll() is called,
        and the return value will be true
    */
    bool wait(SMutex *mutex);

    //! Wakes one thread waiting on the wait condition.
    void wakeupOne();

    //! Wakes all threads waiting on the wait condition.
    void wakeupAll();
};

#endif // SWAITCONDITION_H
