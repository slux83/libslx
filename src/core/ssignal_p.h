/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL_P_H
#define SSIGNAL_P_H

#include "../global/sglobal.h"
#include "../concurrent/smutex.h"
#include "../concurrent/smutexlocker.h"
#include "sapplication.h"
#include "spolicy.h"

class SSlot;

enum SSignalFlag
{
	SSignalFlagThreadSafe = 0x1,
	SSignalFlagThreadUnsafe = 0x2,

	SSignalFlagSyncConnection = 0x4,
	SSignalFlagAsyncConnection = 0x8
};

namespace internalS
{
	/*!
		\internal Base signal class
	*/
	class SAbstractSignal
	{
	protected:
		SMutex signalMutex;

		SSignalFlag flags;

	public:
		/*! Disconnect all slots of the slotTarget argument from this signal.
			Used when the slot object is destroyed
		*/
		virtual void disconnectAll(SSlot *slotTarget) = 0;

		/*!
			Constructor.
			\param SSignalFlag flags. It accepts also a combination of OR

			Default: (SSignalFlagThreadSafe | SSignalFlagSyncConnection)
		*/
		explicit SAbstractSignal(int signalFlags = SSignalFlagThreadSafe | SSignalFlagSyncConnection)
		{
			flags = (SSignalFlag) signalFlags;
		}
	};

	class SAbstractSignalSlotConnection
	{

	};

	/*!
		\internal Abstract signal connection without arguments
	*/
	class SAbstractSignalSlotConnection0 : public SAbstractSignalSlotConnection
	{
	public:
		virtual void fire() = 0;
		virtual SSlot* getTarget() = 0;
	};

	/*!
		\internal Concrete connection object without arguments
	*/
	template <class SSlotType>
	class SSignalSlotConnection0 : public SAbstractSignalSlotConnection0
	{
		typedef void (SSlotType::*CallableMethod)();

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;
		SSignalFlag flags;

	public:
		explicit SSignalSlotConnection0(SSlotType *target, CallableMethod callable, SSignalFlag signalFlags)
		{
			slotTarget = target;
			callableMethod = callable;
			flags = signalFlags;
		}

		virtual void fire()
		{
			SMutexLocker locker((flags & SSignalFlagThreadSafe)? &(slotTarget->slotCallbackLock) : NULL);
			S_USE_VAR(locker);

			if (slotTarget != NULL)
				(slotTarget->*callableMethod)();
			else
				sWarning("internalS::SSignalSlotConnection0::fire() slotTarget is NULL");
		}

		virtual SSlot* getTarget()
		{
			return slotTarget;
		}
	};

	/*!
		\internal Abstract signal connection with one argument
	*/
	template <typename arg1>
	class SAbstractSignalSlotConnection1 : public SAbstractSignalSlotConnection
	{
	public:
		virtual void fire(arg1 a1) = 0;
		virtual SSlot* getTarget() = 0;
	};

	/*!
		\internal Concrete connection object with one argument
	*/
	template <class SSlotType, typename arg1>
	class SSignalSlotConnection1 : public SAbstractSignalSlotConnection1<arg1>
	{
		typedef void (SSlotType::*CallableMethod)(arg1);

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;
		SSignalFlag flags;

	public:
		explicit SSignalSlotConnection1(SSlotType *target, CallableMethod callable, SSignalFlag signalFlags)
		{
			slotTarget = target;
			callableMethod = callable;
			flags = signalFlags;
		}

		virtual void fire(arg1 a1)
		{
			SMutexLocker locker((flags & SSignalFlagThreadSafe)? &(slotTarget->slotCallbackLock) : NULL);
			S_USE_VAR(locker);

			if (slotTarget != NULL)
				(slotTarget->*callableMethod)(a1);
			else
				sWarning("internalS::SSignalSlotConnection1::fire(arg1) slotTarget is NULL");
		}

		virtual SSlot* getTarget()
		{
			return slotTarget;
		}

	};

	/*!
		\internal Abstract signal connection with two argument
		\todo to be implemented
	*/

}

#endif // SSIGNAL_P_H
