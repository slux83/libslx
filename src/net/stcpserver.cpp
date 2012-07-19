/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "stcpserver.h"
#include <arpa/inet.h>
#include <sstream>
#include <cstring>
#include <cerrno>

STcpServer::STcpServer(uint8_t listenQueueSize)
{
	sock = -1;
	this->listenQueueSize = listenQueueSize;
}

STcpServer::~STcpServer()
{
	abort();
}


void STcpServer::abort()
{
	if(sock != -1)
	{
		if (::close(sock) == -1)
			sWarning("STcpServer::abort() failed to close the socket %d. ERRNO=%d", sock, errno);
	}

	sock = -1;
}

bool STcpServer::listen(uint16_t port, SAbstractSocket::SocketError *error)
{
	struct sockaddr_in serverAddr, clientAddr;
	socklen_t clientAddrLen;

	S_ASSERT_MSG(port > 0, "STcpServer::listen() port must be greater than zero");

	if (error) *error = SAbstractSocket::SocketErrorNoError;

	if ((sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_IP)) < 0)
	{
		abort();

		if (error) *error = SAbstractSocket::SocketErrorCannotCreateSocket;

		return false;
	}

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(port);

	if (::bind(sock, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) < 0)
	{
		abort();

		if (error) *error = SAbstractSocket::SocketErrorCannotBindSocket;

		return false;

	}

	if (::listen(sock, (int)listenQueueSize) < 0)
	{
		sWarning("STcpServer::listen() failed to linten the socket %d. ERRNO=%d", sock, errno);

		abort();

		if (error) *error = SAbstractSocket::SocketErrorCannotListen;

		return false;
	}

	clientAddrLen = sizeof(clientAddr);
	S_FOREVER
	{
		int clientSocket = accept(sock, (struct sockaddr*) &clientAddr, &clientAddrLen);

		if (clientSocket < 0)
		{
			sWarning("STcpServer::listen() failed to accept. ERRNO=%d", errno);

			continue;
		}

		STcpSocket incomingClientSocket(clientSocket);

		//Notify the new connection to subscribers
		newConnectionReady.fire(incomingClientSocket);
	}


	return true;

}
