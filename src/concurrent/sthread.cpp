/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sthread.h"
#include "smutexlocker.h"
#include "../global/sglobal.h"
#include <cstdio>

SThread::SThread(const std::string &threadName) : runStatus(false)
{
	thread = 0;
    name = threadName;
}

SThread::~SThread()
{
}

bool SThread::join()
{
    SMutexLocker locker(&mutex);
    S_USE_VAR(locker);

    if (!runStatus)
        return true;

    int returnedValue = pthread_join(thread, NULL);

    if (returnedValue != 0)
        sWarning("SThread::join() failed with error code %d", returnedValue);

    return (returnedValue == 0);
}

void SThread::start()
{
    SMutexLocker locker(&mutex);
    S_USE_VAR(locker);

    if (runStatus)  //Thread is already runnnig
    {
		sWarning("SThread::start() SThread [ID=%u] already running", (uint32_t) thread);
        return;
    }

    //Create and run the thead
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

    int returnedValue = pthread_create(&thread, &attr, SThread::starter, (void*) this);
    if (returnedValue == EPERM)
    {
        // Run the thread without the attr
        pthread_create(&thread, NULL, SThread::starter, (void*) this);
        runStatus = true;
    }
    else if (returnedValue == 0)
    {
        //Okay, thread started
        runStatus = true;
    }
    else
    {
        sCritical("SThread::start() cannot create pthread. Returned value: %d", returnedValue);
    }

    if (runStatus && name.length() == 0)
    {
        nameMutex.lock();

        //Choose a name
        char tmp[128];
		sprintf(tmp, "Thread-%u", (uint32_t)thread);
        name = tmp;

        nameMutex.unlock();
    }

    pthread_attr_destroy(&attr);
}

bool SThread::isRunning()
{
    SMutexLocker locker(&mutex);
    S_USE_VAR(locker);

    return runStatus;
}

void SThread::run()
{
    //Nothing to do here
}

std::string SThread::getName() const
{
    SMutexLocker locker(&nameMutex);
    S_USE_VAR(locker);

    return name;
}

void *SThread::starter(void *arg)
{
    S_ASSERT(arg != NULL);

    SThread *creator = reinterpret_cast<SThread*>(arg);
    creator->run();

    creator->runStatus = false;
    return 0;
}

