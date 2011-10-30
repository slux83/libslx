/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include "../../src/core/ssignal.h"
#include "../../src/core/sapplication.h"

//Internal Slot class
class MySlotClass2 : public SSlot
{
public:
	int globalVar;

	MySlotClass2()  { globalVar = 0; }

	void slot0(int a) { globalVar += a; }
	void slot1(int a) { globalVar += a; }
	void slot2(int a) { globalVar += a; }
	void slot3(int a) { globalVar += a; }
	void slot4(int a) { globalVar += a; }
	void slot5(int a) { globalVar += a; }
	void slot6(int a) { globalVar += a; }
	void slot7(int a) { globalVar += a; }
	void slot8(int a) { globalVar += a; }
	void slot9(int a) { globalVar += a; }
};

/*
 Example/Benchmark for signal slots
*/
int main (int argc, char** argv)
{
	S_USE_VAR(argc);
	S_USE_VAR(argv);
	SApplication::init();
	SApplication::getInstance()->start();

	SSignal1<int> s1/*(SSignalFlagThreadUnsafe | SSignalFlagAsyncConnection)*/;

	for (int i=0; i<1000; ++i)
		s1.fire(12);

	MySlotClass2 msc2;

	s1.connect(&msc2, &MySlotClass2::slot0);
	s1.connect(&msc2, &MySlotClass2::slot1);
	s1.connect(&msc2, &MySlotClass2::slot2);
	s1.connect(&msc2, &MySlotClass2::slot3);
	s1.connect(&msc2, &MySlotClass2::slot4);

	for (int i=0; i<1000; ++i)
		s1.fire(1);

	s1.connect(&msc2, &MySlotClass2::slot5);
	s1.connect(&msc2, &MySlotClass2::slot6);
	s1.connect(&msc2, &MySlotClass2::slot7);
	s1.connect(&msc2, &MySlotClass2::slot8);
	s1.connect(&msc2, &MySlotClass2::slot9);

	for (int i=0; i<1000; ++i)
		s1.fire(1);

	/* This is called when the slot is destroyed */
	s1.disconnectAll(&msc2);

	return 0;
}
