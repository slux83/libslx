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

class SSlot;
class SVariant;

enum SSignalFlag
{
	SSignalFlagThreadSafe = 0x1,
	SSignalFlagThreadUnsafe = 0x2,

	SSignalFlagSyncConnection = 0x4,
	SSignalFlagAsyncConnection = 0x8
};

/*!
	\namespace internal stuff
*/
namespace internalS
{
	/*!
		\internal Base signal class
	*/
	class SAbstractSignal
	{
	protected:
		//! The mutex of this signal
		SMutex signalMutex;

		//! The flags of this signal
		SSignalFlag flags;

	public:
		//! Destructor
		virtual ~SAbstractSignal() {}

		/*! Disconnect all slots of the slotTarget argument from this signal.
			Used when the slot object is destroyed
		*/
		virtual void disconnectAll(SSlot *slotTarget) = 0;

		/*!
			Clear all connections
		*/
		virtual void clear() = 0;
		/*!
			Constructor.
			\param SSignalFlag flags. It accepts also a combination of OR

			Default: (SSignalFlagThreadSafe | SSignalFlagSyncConnection)
		*/
		explicit SAbstractSignal(int signalFlags = SSignalFlagThreadSafe | SSignalFlagSyncConnection)
		{
			//Checks if the user sets both async and sync calls
			if ((signalFlags & SSignalFlagAsyncConnection) && (signalFlags & SSignalFlagSyncConnection))
			{
				sWarning("SAbstractSignal() invalid Connection policy flags. Setting SSignalFlagSyncConnection as default");
				signalFlags &= ~SSignalFlagAsyncConnection;	//Prefer sync
			}

			//Checks if the user sets both thread safe and unsafe calls
			if ((signalFlags & SSignalFlagThreadUnsafe) && (signalFlags & SSignalFlagThreadSafe))
			{
				sWarning("SAbstractSignal() invalid Thread lock policy flags. Setting SSignalFlagThreadSafe as default");
				signalFlags &= ~SSignalFlagThreadUnsafe;
			}

			//Checks if the user sets neither async or sync calls
			if (!(signalFlags & SSignalFlagAsyncConnection) && !(signalFlags & SSignalFlagSyncConnection))
			{
				sWarning("SAbstractSignal() invalid Connection policy flags. Setting SSignalFlagSyncConnection as default");
				signalFlags |= SSignalFlagSyncConnection;	//Prefer sync
			}

			//Checks if the user sets neither thread safe and unsafe calls
			if (!(signalFlags & SSignalFlagThreadUnsafe) && !(signalFlags & SSignalFlagThreadSafe))
			{
				sWarning("SAbstractSignal() invalid Thread lock policy flags. Setting SSignalFlagThreadSafe as default");
				signalFlags |= SSignalFlagThreadSafe;
			}

			flags = (SSignalFlag) signalFlags;
		}

		virtual void asyncExec(const std::map<int, SVariant> &args) = 0;
	};

	class SAbstractSignalSlotConnection
	{

	public:
		//! Destructor
		virtual ~SAbstractSignalSlotConnection() {}

	};

	/*!
		\internal Abstract signal connection without arguments
	*/
	class SAbstractSignalSlotConnection0 : public SAbstractSignalSlotConnection
	{
	public:
		//! Destructor
		virtual ~SAbstractSignalSlotConnection0() {}

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
		\internal Abstract signal connection with two arguments
	*/
	template <typename arg1, typename arg2>
	class SAbstractSignalSlotConnection2 : public SAbstractSignalSlotConnection
	{
	public:
		virtual void fire(arg1 a1, arg2 a2) = 0;
		virtual SSlot* getTarget() = 0;
	};

	/*!
		\internal Concrete connection object with two arguments
	*/
	template <class SSlotType, typename arg1, typename arg2>
	class SSignalSlotConnection2 : public SAbstractSignalSlotConnection2<arg1, arg2>
	{
		typedef void (SSlotType::*CallableMethod)(arg1, arg2);

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;
		SSignalFlag flags;

	public:
		explicit SSignalSlotConnection2(SSlotType *target, CallableMethod callable, SSignalFlag signalFlags)
		{
			slotTarget = target;
			callableMethod = callable;
			flags = signalFlags;
		}

		virtual void fire(arg1 a1, arg2 a2)
		{
			SMutexLocker locker((flags & SSignalFlagThreadSafe)? &(slotTarget->slotCallbackLock) : NULL);
			S_USE_VAR(locker);

			if (slotTarget != NULL)
				(slotTarget->*callableMethod)(a1, a2);
			else
				sWarning("internalS::SSignalSlotConnection2::fire(arg1, arg2) slotTarget is NULL");
		}

		virtual SSlot* getTarget()
		{
			return slotTarget;
		}

	};
}

#endif // SSIGNAL_P_H
