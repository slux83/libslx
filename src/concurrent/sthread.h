/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STHREAD_H
#define STHREAD_H

#include <pthread.h>
#include "smutex.h"

/*!
    \brief SMutex class represent a Thread class that can be subclassed.
    The subclass should reimplement the run().
*/
class SThread
{
public:
    //! Constructor
    SThread();

    //! Destructor
    virtual ~SThread();

    //! Join function that wraps the pthead join behaviour
    void join();

    //! Start the thread executing the run() function
    void start();

    /*! Return the status of this thread
        \return true if the thread is running, false otherwise
    */
    bool isRunning();

protected:
    /*! Thread run function. The default implementation do nothing.
        \todo Implements an internal thread event loop
    */
    virtual void run();

private:
    //! Stores the status of the thread (true means runnnig)
    bool runStatus;

    //! Internal mutex to prevent concurrent access to local variables
    SMutex mutex;

    //! The pthread real thread ID
    pthread_t thread;

    //! Function called by pthread_create
    static void *starter(void *arg);

};

#endif // STHREAD_H
