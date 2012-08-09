/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SUDPSOCKET_H
#define SUDPSOCKET_H

#include "stcpsocket.h"
#include "../global/sglobal.h"

class SUdpSocket : public SAbstractSocket
{
private:
	std::string remoteHost;
	int sock;

public:

	/*! Set the remote host. It can be an IPv4 or IPv6
		address	like "127.0.0.1", "::1" or an host name "example.org".
		\param host
		\param error type if false is returned
		\return true on success, false otherwise (with error setted if not NULL)
	*/
	bool setRemoteHost(const std::string &remoteHost, SocketError *error = NULL);

	//! Constructor
	SUdpSocket();

	//! Distructor
	virtual ~SUdpSocket();

	//! \sa SAbstractSocket::abort()
	virtual void abort();

	//! \sa SAbstractSocket::writeData()
	virtual int64_t writeData(const char *data, size_t size);

	/*!
		\sa SAbstractSocket::readData()
		\note if maxSize is not enough big, part of the udp packet is lost
	*/
	virtual int64_t readData(char* data, uint32_t maxSize);

	/*!
		\param the port number to bind the socket
		\param the address to bind the socket. Default any address
		\return true on success, false otherwise
	*/
	bool bind(uint16_t port, const std::string &address = "0.0.0.0");

};

#endif // SUDPSOCKET_H
