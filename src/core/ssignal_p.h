/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL_P_H
#define SSIGNAL_P_H

namespace internalS
{
	/*!
		\internal Abstract signal connection without arguments
	*/
	class SAbstractSignalSlotConnection0
	{
	public:
		virtual void fire() = 0;
	};

	template <class SSlotType>
	class SSignalSlotConnection0 : public SAbstractSignalSlotConnection0
	{
		typedef void (SSlotType::*CallableMethod)();

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;

	public:
		explicit SSignalSlotConnection0(SSlotType *target, CallableMethod callable)
		{
			slotTarget = target;
			callableMethod = callable;
		}

		virtual void fire()
		{
			(slotTarget->callableMethod)();
		}
	};

	/*!
		\internal Abstract signal connection with one argument
	*/
	template <typename arg1>
	class SAbstractSignalSlotConnection1
	{
	public:
		virtual void fire(arg1 a1) = 0;
	};

	template <class SSlotType, typename arg1>
	class SSignalSlotConnection1 : public SAbstractSignalSlotConnection1<arg1>
	{
		typedef void (SSlotType::*CallableMethod)();

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;

	public:
		explicit SSignalSlotConnection1(SSlotType *target, CallableMethod callable)
		{
			slotTarget = target;
			callableMethod = callable;
		}

		virtual void fire(arg1 a1)
		{
			(slotTarget->callableMethod)(a1);
		}
	};

	/*!
		\internal Abstract signal connection with two argument
	*/
	template <typename arg1, typename arg2>
	class SAbstractSignalSlotConnection2
	{
	public:
		virtual void fire(arg1 a1, arg2 a2) = 0;
	};

	template <class SSlotType, typename arg1, typename arg2>
	class SSignalSlotConnection2 : public SAbstractSignalSlotConnection2<arg1, arg2>
	{
		typedef void (SSlotType::*CallableMethod)();

	private:
		SSlotType *slotTarget;
		CallableMethod callableMethod;

	public:
		explicit SSignalSlotConnection2(SSlotType *target, CallableMethod callable)
		{
			slotTarget = target;
			callableMethod = callable;
		}

		virtual void fire(arg1 a1, arg2 a2)
		{
			(slotTarget->callableMethod)(a1, a2);
		}
	};
}

#endif // SSIGNAL_P_H
