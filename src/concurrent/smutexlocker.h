/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SMUTEXLOCKER_H
#define SMUTEXLOCKER_H

class SMutex;

/*!
    \brief SMutexLocker class is a convenience class that simplifies locking and unlocking mutexes.
    The Mutex passed as argument is locked and when the SMutexLocker
    is destroyed, the mutex is unlocked.
*/
class SMutexLocker
{
private:
    //! The mutex passed to the constructor method.
    SMutex *mutex;

	//! Generally false, but become true after the calling of unlock()
	bool alreadyUnlocked;

public:
    /*! Construct the object and lock the mutex.
        \param mutex shall be a valid SMutex
    */
    SMutexLocker(SMutex *mutex);

    //! Destruct the object and unlocks the mutex that was locked in the constructor.
    virtual ~SMutexLocker();

	//! Unlock the mutex
	void unlock();
};

#endif // SMUTEXLOCKER_H
