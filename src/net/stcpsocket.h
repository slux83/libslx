/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STCPSOCKET_H
#define STCPSOCKET_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>

#include "sabstractsocket.h"

/*!
	\brief TCP Socket implementation IPv4/IPv6
	\note This implementation is not thread-safe
*/
class STcpSocket : public SAbstractSocket
{
private:
	int sock;

public:
	//! Constructor
	STcpSocket();

	//! Distructor
	virtual ~STcpSocket();

	/*! Open the connection to the host and the specific port. The host can be an IPv4 or IPv6
		address	like "127.0.0.1", "::1" or an host name "example.org".
		\param host
		\param port
		\param error type if false is returned
		\return true on success, false otherwise (with error setted if not NULL)
	*/
	bool connect(const std::string &host, uint16_t port, SocketError *error = NULL);

	//! \sa SAbstractSocket::abort()
	virtual void abort();

	//! Connection check. \return true if the socket is connected, false otherwise
	inline bool isConnected() const { return (sock != -1); }

	//! \sa SAbstractSocket::writeData()
	virtual int64_t writeData(const char *data, size_t size);

	//! \sa SAbstractSocket::readData()
	virtual int64_t readData(char* data, uint32_t maxSize);

	//! Get the remote host or an empty string on error
	std::string getRemoteHost() const;

	//! Get the remote port, or 0 on error
	uint16_t getRemotePort() const;

};

#endif // STCPSOCKET_H
