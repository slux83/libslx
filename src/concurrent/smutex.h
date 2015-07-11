/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SMUTEX_H
#define SMUTEX_H

#include <pthread.h>
#include <cerrno>

/*!
    \brief Recursive mutex implementation
    The constructor takes the type of the mutex.
    Default is reentrant (Recursive)
*/
class SMutex
{
public:
    /*!
        Recursive: setup the mutex for recursive mode
        NonRecursive: setup the mutex for non recursive mode
    */
    enum RecursionMode { Recursive, NonRecursive };

private:
    //! Internal pthread mutex
    pthread_mutex_t mutex;

    //! The recursive mode
    SMutex::RecursionMode recursionMode;

public:

    /*! Constructor
        \param recursionMode (Defaut Recursive)
    */
    SMutex(SMutex::RecursionMode recursionMode = SMutex::Recursive);

    /*! Deconstructor
        \warning Destroying a locked mutex may result in undefined behavior.
    */
    virtual ~SMutex();

    /*!
        Locks the mutex.
        If another thread has locked the mutex then this call
        will block until that thread has unlocked it.
    */
    void lock();

    /*!
        Unlocks the mutex.
        Attempting to unlock a mutex in a different thread to the one that
        locked it results in an error. Unlocking a mutex that is not
        locked results in undefined behavior.
    */
    void unlock();

    /*!
        Attempts to lock the mutex.
        If the lock was obtained, this function returns true.
        If another thread has locked the mutex, this function returns false immediately.
        If the lock was obtained, the mutex must be unlocked with unlock()
        before another thread can successfully lock it.
    */
    bool tryLock();

    //! \return the recursionMode
    inline SMutex::RecursionMode getRecursiveMode() const { return recursionMode; }
};

#endif // SMUTEX_H
