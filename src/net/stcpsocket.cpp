/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#include "stcpsocket.h"
#include <arpa/inet.h>
#include <sstream>
#include <cstring>
#include <cerrno>

STcpSocket::STcpSocket() : SAbstractSocket(SocketTypeTcp)
{
	sock = -1;
}

STcpSocket::~STcpSocket()
{
	abort();
}

void STcpSocket::abort()
{
	if(sock != -1)
	{
		if (::close(sock) == -1)
			sWarning("STcpSocket::abort() failed to close the socket %d. ERRNO=%d", sock, errno);
	}

	sock = -1;
}

bool STcpSocket::connect(const std::string &host, uint16_t port, SocketError *error)
{
	struct addrinfo hints;
	struct addrinfo *server;
	int status = 0;
	std::string portString;
	std::stringstream out;

	out << port;
	portString = out.str();

	if (error) *error = SocketErrorUnknown;

	// Resolve the name if needed (IPv4 or IPv6 we don't care)
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;	// TCP protocol

	status = getaddrinfo(host.c_str(), portString.c_str(), &hints, &server);
	if(status != 0 || server == NULL)
	{
		abort();

		if (error) *error = SocketErrorCannotResolve;

		return false;
	}

	// Create the socket
	if((sock = socket(server->ai_family, SOCK_STREAM, IPPROTO_TCP)) < 0)
	{
		freeaddrinfo(server);
		abort();

		if (error) *error = SocketErrorCannotCreateSocket;

		return false;
	}

	// Connect
	if( ::connect(sock, server->ai_addr,server->ai_addrlen) < 0 )
	{
		freeaddrinfo(server);
		abort();

		if (error) *error = SocketErrorConnectionRefused;

		return false;
	}

	freeaddrinfo(server);

	return true;
}

int64_t STcpSocket::writeData(const char *data, size_t size)
{
	int64_t writtenBytes = -1;

	S_ASSERT_MSG(data != NULL, "STcpSocket::writeData() NULL data buffer");

	if (size == 0)
	{
		sWarning("STcpSocket::writeData() zero data size");
		return writtenBytes;
	}

	if (!isConnected())
	{
		sWarning("STcpSocket::writeData() socket not connected");
		return writtenBytes;
	}

	if ((writtenBytes = ::write(sock, data, size)) < 0)
	{
		writtenBytes = -1;
		abort();
	}

	return writtenBytes;
}

int64_t STcpSocket::readData(char* data, uint32_t maxSize)
{
	int64_t readBytes = -1;

	S_ASSERT_MSG(data != NULL, "STcpSocket::readData() NULL data buffer");

	if (maxSize == 0)
	{
		sWarning("STcpSocket::readData() zero max data size");
		return readBytes;
	}

	if (!isConnected())
	{
		sWarning("STcpSocket::readData() socket not connected");
		return readBytes;
	}

	if ((readBytes = ::read(sock, data, maxSize)) < 0)
	{
		readBytes = -1;
		abort();
	}

	return readBytes;
}

std::string STcpSocket::getRemoteHost() const
{
	socklen_t len;
	struct sockaddr_storage addr;
	char ipstr[INET6_ADDRSTRLEN];

	if (!isConnected())
	{
		sWarning("STcpSocket::getRemoteHost() socket not connected");
		return "";
	}

	len = sizeof addr;
	getpeername(sock, (struct sockaddr*)&addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else
	{ // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}

	return ipstr;
}

uint16_t STcpSocket::getRemotePort() const
{
	socklen_t len;
	struct sockaddr_storage addr;
	uint16_t port = 0;

	if(!isConnected())
	{
		sWarning("STcpSocket::getRemotePort() socket not connected");
		return 0;
	}

	len = sizeof addr;
	getpeername(sock, (struct sockaddr*)&addr, &len);

	// deal with both IPv4 and IPv6:
	if (addr.ss_family == AF_INET)
	{
		struct sockaddr_in *s = (struct sockaddr_in *)&addr;
		port = ntohs(s->sin_port);
	}
	else
	{ // AF_INET6
		struct sockaddr_in6 *s = (struct sockaddr_in6 *)&addr;
		port = ntohs(s->sin6_port);
	}

	return port;

}
