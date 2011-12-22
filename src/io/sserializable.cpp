/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sserializable.h"

SSerializable::SSerializable()
{
}

bool SSerializable::isLittleEndian()
{
	int tester = 0x0A;
	return (*((char *)&tester)) == 0x0A;
}
