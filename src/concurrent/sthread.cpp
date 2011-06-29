#include "sthread.h"
#include "smutexlocker.h"
#include "../global/sglobal.h"

SThread::SThread() : runStatus(false)
{

}

SThread::~SThread()
{

}

void SThread::join()
{

}

void SThread::start()
{
    SMutexLocker locker(&mutex);
    S_USE_VAR(locker);

    if (runStatus)  //Thread is already runnnig
    {
        sWarning("SThread::start() SThread [ID=%d] already running", thread);
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

    //TODO: remove debug
    sDebug("SThread::start() New thread started [ID=%d]", thread);

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

    //TODO: remove debug
    sDebug("SThread::run() New thread started [ID=%d]", thread);
    sDebug("SThread::run() Exiting... [ID=%d]", thread);
}

void *SThread::starter(void *arg)
{
    S_ASSERT(arg != NULL);

    SThread *creator = reinterpret_cast<SThread*>(arg);
    creator->run();

    return 0;
}
