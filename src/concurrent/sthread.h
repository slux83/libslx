/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STHREAD_H
#define STHREAD_H

#include <pthread.h>
#include <string>
#include "smutex.h"

/*!
    \brief SMutex class represent a Thread class that can be subclassed.
    The subclass should reimplement the run().
    \threadsafe All functions are thread safe.
*/
class SThread
{
public:
    /*! Constructor.
        \param threadName the name of the thread.
        If an empty string is passed (default), the implementation sets the
        name as Thread-<ID> after the call of the start function.
    */
    SThread(const std::string &threadName = "");

    //! Destructor
    virtual ~SThread();

    /*! Join function that wraps the pthead join behaviour.
        \return true on success or if the thread is not running, false on error
    */
    bool join();

    //! Start the thread executing the run() function
    void start();

    /*! Return the status of this thread
        \return true if the thread is running, false otherwise

        \warning Because this function is thread-safe, calling this function from \
        the subclass casue a dead-lock. Use \ref runStatus instead.
    */
    bool isRunning();

    /*! Get the name of the thread.
        \return the thread name
    */
    std::string getName() const;

protected:
    /*! Thread run function. The default implementation do nothing.
        \todo Implements an internal thread event loop
    */
    virtual void run();

    //! Stores the name of the thread (default Thread-<ID>)
    std::string name;

    //! Stores the status of the thread (true means runnnig)
    bool runStatus;

private:
    //! Internal mutex to prevent concurrent access to local variables
    SMutex mutex;

    //! Internal mutex for \ref name
    mutable SMutex nameMutex;

    //! The pthread real thread ID
    pthread_t thread;

    //! Function called by pthread_create
    static void *starter(void *arg);

};

#endif // STHREAD_H
