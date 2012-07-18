/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/
#include <unistd.h>
#include <string.h>
#include "../../src/core/sapplication.h"
#include "../../src/net/stcpsocket.h"

#define getIndexData "get index.html\n"
#define getIndexDataSize 15

/*
	Example of STcpSocket usage
*/
int main (int argc, char** argv)
{
	S_USE_VAR(argc);
	S_USE_VAR(argv);

	SApplication::init();

	STcpSocket *socket = new STcpSocket();

	SAbstractSocket::SocketError error = (SAbstractSocket::SocketError) -1;
	bool retVal = socket->connect("::1", 80, &error);

	S_ASSERT_MSG(retVal, "connect returned false");
	S_ASSERT_MSG(error == SAbstractSocket::SocketErrorUnknown, "error is not unknown");

	S_ASSERT_MSG(socket->isConnected(), "socket not connected");

	int64_t writtenBytes = socket->writeData(getIndexData, getIndexDataSize);

	S_ASSERT_MSG(writtenBytes == getIndexDataSize, "written not enough bytes");

	char buff[4096];

	::memset(buff, 0, 4096);

	int64_t readBytes = socket->readData(buff, 4095);

	S_ASSERT_MSG(readBytes != -1, "readBytes is -1");

	sDebug("buffer(size %lld): %s ", readBytes, buff);

	delete socket;
	socket = NULL;

	SApplication::getInstance()->shutdown();

	return 0;
}
