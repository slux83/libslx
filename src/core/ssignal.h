/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SSIGNAL_H
#define SSIGNAL_H

#include <list>

/*!
	\brief Abstract base class for Signal and slot connections without arguments
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

class SSignal0
{
	typedef std::list<SAbstractSignalSlotConnection0*> SignalList;
	typedef typename SignalList::const_iterator SignalListConstIterator;

private:
	SignalList connections;

public:
	template <class SSlotType>
	void connect(SSlotType *slotTarget, void (SSlotType::*callableMethod)())
	{
		SSignalSlotConnection0<SSlotType> *conn =
				new SSignalSlotConnection0<SSlotType> (slotTarget, callableMethod);
		connections.push_back(conn);
	}

	void fire()
	{
		 SignalListConstIterator it = connections.begin();
		 SignalListConstIterator end = connections.end();

		while (it != end)
		{
			(*it)->fire();
			it++;
		}
	}
};
/* PROTOTYPE: */
/*

#include <iostream>
#include <list>


template <typename arg1, typename arg2>
class AbstractSlotConnection
{
public:
	virtual void fire(arg1 a1, arg2 a2) = 0;
};

template <class SlotType, typename arg1, typename arg2>
class SlotConnection : public AbstractSlotConnection<arg1, arg2>
{
	typedef void (SlotType::*Callable)(arg1, arg2);

private:
	SlotType *target;
	Callable call;

public:
	explicit SlotConnection(SlotType *t, Callable c)
	{
		target = t;
		call = c;
	}

	void fire(arg1 a1, arg2 a2)
	{
		(target->*call)(a1, a2);
	}
};

template <typename arg1, typename arg2>
class Slot
{
	typedef std::list< AbstractSlotConnection<arg1, arg2> *> CallableVector;
	typedef typename CallableVector::const_iterator VectorConstIterator;

private:
	CallableVector funs;

public:
	template <class SlotType>
	void bind(SlotType *obj, void (SlotType::*callable)(arg1, arg2))
	{
		SlotConnection<SlotType, arg1, arg2> *conn = new SlotConnection<SlotType, arg1, arg2>(obj, callable);
		funs.push_back(conn);
	}

	void fire(arg1 a, arg2 b)
	{
		VectorConstIterator it = funs.begin();
		VectorConstIterator end = funs.end();

		while (it != end)
		{
			(*it)->fire(a, b);
			it++;
		}
	}
};

class TestSlotString1
{
public:
	void fire(int a, const std::string &b)
	{
		std::cout << "TestSlotString1::fire()\t a" << "=" << a << "\t b=" << b << std::endl;
	}
};


class TestSlotString2
{
public:
	void fire(int a, const std::string &b)
	{
		std::cout << "TestSlotString2::fire()\t a" << "=" << a << "\t b=" << b << std::endl;
	}
};


class TestSlotString3
{
public:
	void fire(int a, const std::string &b)
	{
		std::cout << "TestSlotString3::fire()\t a" << "=" << a << "\t b=" << b << std::endl;
	}
};


class TestSlotString4
{
public:
	void fire4(int a, const std::string &b)
	{
		std::cout << "TestSlotString4::fire4()\t a" << "=" << a << "\t b=" << b << std::endl;
	}

	void fire4bis(double a, const char* b)
	{
		std::cout << "TestSlotString4::fire4bis()\t a" << "=" << a << "\t b=" << b << std::endl;
	}
};



int main(int argc, char *argv[])
{
	std::cout << "main() start" << std::endl;

	TestSlotString1 tss1;
	TestSlotString2 tss2;
	TestSlotString3 tss3;
	TestSlotString4 tss4;


	Slot<int, const std::string&> slot1;
	Slot<double, const char*> slot2;

	slot1.bind(&tss1, &TestSlotString1::fire);
	slot1.bind(&tss2, &TestSlotString2::fire);
	slot1.bind(&tss3, &TestSlotString3::fire);
	slot1.bind(&tss4, &TestSlotString4::fire4);
	slot1.bind(&tss4, &TestSlotString4::fire4);

	slot2.bind(&tss4, &TestSlotString4::fire4bis);

	slot1.fire(666, "fuck the hell!");
	slot1.fire(999, "fuck the heaven!");

	slot2.fire(3.1415926535897931, "another test");

	return 0;
}

*/

#endif // SSIGNAL_H
