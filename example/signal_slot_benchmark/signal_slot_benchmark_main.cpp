/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include <iostream>
#include <cstdio>
#include "../../src/core/ssignal.h"
#include "../../src/core/sapplication.h"
#include "../../src/core/stime.h"

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

	SSignal1<int> s1;
	SSignal1<int> s2(SSignalFlagThreadUnsafe | SSignalFlagSyncConnection);
	MySlotClass2 msc2;

	STimestamp checkpointOneMillionEmptyBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s1.fire(12);

	STimestamp checkpointOneMillionEmptyEnd = STime::now();

	s1.connect(&msc2, &MySlotClass2::slot0);
	s1.connect(&msc2, &MySlotClass2::slot1);
	s1.connect(&msc2, &MySlotClass2::slot2);
	s1.connect(&msc2, &MySlotClass2::slot3);
	s1.connect(&msc2, &MySlotClass2::slot4);

	STimestamp checkpointOneMillion5SlotsBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s1.fire(1);

	STimestamp checkpointOneMillion5SlotsEnd = STime::now();

	s1.connect(&msc2, &MySlotClass2::slot5);
	s1.connect(&msc2, &MySlotClass2::slot6);
	s1.connect(&msc2, &MySlotClass2::slot7);
	s1.connect(&msc2, &MySlotClass2::slot8);
	s1.connect(&msc2, &MySlotClass2::slot9);

	STimestamp checkpointOneMillion10SlotsBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s1.fire(1);

	STimestamp checkpointOneMillion10SlotsEnd = STime::now();



	STimestamp checkpointOneMillionEmptyUnsafeBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s2.fire(12);

	STimestamp checkpointOneMillionEmptyUnsafeEnd = STime::now();

	s2.connect(&msc2, &MySlotClass2::slot0);
	s2.connect(&msc2, &MySlotClass2::slot1);
	s2.connect(&msc2, &MySlotClass2::slot2);
	s2.connect(&msc2, &MySlotClass2::slot3);
	s2.connect(&msc2, &MySlotClass2::slot4);

	STimestamp checkpointOneMillion5SlotsUnsafeBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s2.fire(1);

	STimestamp checkpointOneMillion5SlotsUnsafeEnd = STime::now();

	s2.connect(&msc2, &MySlotClass2::slot5);
	s2.connect(&msc2, &MySlotClass2::slot6);
	s2.connect(&msc2, &MySlotClass2::slot7);
	s2.connect(&msc2, &MySlotClass2::slot8);
	s2.connect(&msc2, &MySlotClass2::slot9);

	STimestamp checkpointOneMillion10SlotsUnsafeBegin = STime::now();

	for (int i=0; i<1000000; ++i)
		s2.fire(1);

	STimestamp checkpointOneMillion10SlotsUnsafeEnd = STime::now();


	/* This is called when the slot is destroyed */
	s2.disconnectAll(&msc2);

	//Print the summary of the benchmark
	char buff[512];
	std::cout << "-----------------------------------------" << std::endl;
	std::cout << "----------- Benchmark Summary -----------" << std::endl;
	std::cout << "-----------------------------------------" << std::endl << std::endl;

	std::cout << "* Fire of 1 million of signals without connected slots *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-SAFE SYNC-CONNECTION]",
			abs(checkpointOneMillionEmptyBegin.sec - checkpointOneMillionEmptyEnd.sec),
			abs(checkpointOneMillionEmptyBegin.usec - checkpointOneMillionEmptyEnd.usec));
	std::cout << buff << std::endl << std::endl;

	std::cout << "* Fire of 1 million of signals with 5 connected slots  *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-SAFE SYNC-CONNECTION]",
			abs(checkpointOneMillion5SlotsBegin.sec - checkpointOneMillion5SlotsEnd.sec),
			abs(checkpointOneMillion5SlotsBegin.usec - checkpointOneMillion5SlotsEnd.usec));
	std::cout << buff << std::endl << std::endl;

	std::cout << "* Fire of 1 million of signals with 10 connected slots *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-SAFE SYNC-CONNECTION]",
			abs(checkpointOneMillion10SlotsBegin.sec - checkpointOneMillion10SlotsEnd.sec),
			abs(checkpointOneMillion10SlotsBegin.usec - checkpointOneMillion10SlotsEnd.usec));
	std::cout << buff << std::endl << std::endl;


	std::cout << "* Fire of 1 million of signals without connected slots *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-UNSAFE SYNC-CONNECTION]",
			abs(checkpointOneMillionEmptyUnsafeBegin.sec - checkpointOneMillionEmptyUnsafeEnd.sec),
			abs(checkpointOneMillionEmptyUnsafeBegin.usec - checkpointOneMillionEmptyUnsafeEnd.usec));
	std::cout << buff << std::endl << std::endl;

	std::cout << "* Fire of 1 million of signals with 5 connected slots  *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-UNSAFE SYNC-CONNECTION]",
			abs(checkpointOneMillion5SlotsUnsafeBegin.sec - checkpointOneMillion5SlotsUnsafeEnd.sec),
			abs(checkpointOneMillion5SlotsUnsafeBegin.usec - checkpointOneMillion5SlotsUnsafeEnd.usec));
	std::cout << buff << std::endl << std::endl;

	std::cout << "* Fire of 1 million of signals with 10 connected slots *" << std::endl;
	sprintf(buff, "sec=%d\tusec=%06d\t[THREAD-UNSAFE SYNC-CONNECTION]",
			abs(checkpointOneMillion10SlotsUnsafeBegin.sec - checkpointOneMillion10SlotsUnsafeEnd.sec),
			abs(checkpointOneMillion10SlotsUnsafeBegin.usec - checkpointOneMillion10SlotsUnsafeEnd.usec));
	std::cout << buff << std::endl << std::endl;

	return 0;
}
