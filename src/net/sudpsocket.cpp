/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "sudpsocket.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

SUdpSocket::SUdpSocket() : SAbstractSocket(SocketTypeUdp)
{
}

bool SUdpSocket::setRemoteHost(const std::string &remoteHost, SocketError *error)
{
	this->remoteHost = remoteHost;

	//We try to resolve the name if needed (IPv4 or IPv6 we don't care)
	struct addrinfo hints;
	struct addrinfo *server;
	int status = 0;

	memset (&hints, 0, sizeof (hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;	// UDP protocol

	status = getaddrinfo(remoteHost.c_str(), NULL, &hints, &server);
	if(status != 0 || server == NULL)
	{
		abort();

		if (error) *error = SocketErrorCannotResolve;

		return false;
	}
}

SUdpSocket::~SUdpSocket()
{

}

void SUdpSocket::abort()
{

}

int64_t SUdpSocket::writeData(const char *data, size_t size)
{

}

int64_t SUdpSocket::readData(char* data, uint32_t maxSize)
{

}

bool SUdpSocket::bind(uint16_t port, const std::string &address)
{

}
