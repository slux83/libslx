/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef STCPSERVER_H
#define STCPSERVER_H

#include "../global/sglobal.h"
#include "../core/ssignal.h"
#include "stcpsocket.h"
#include "sabstractsocket.h"

/*!
	\brief TCP Server implementation IPv4/IPv6
	\note This implementation is not thread-safe
*/
class STcpServer
{

private:
	int sock;
	std::string serverAddress;
	uint16_t serverPort;
	int listenQueueSize;

public:
	/*! Signal to notify incoming connections
		\note The STcpSocket should be handled by one and only one dedicated thread.
	*/
	SSignal1<STcpSocket> newConnectionReady;

	/*! Constructor
		Sets the maximum number of pending accepted connections to lintenQueueSize.
		The TCP Serevr will accept no more than listenQueueSize incoming connections.
		\arg lintenQueueSize (default 30)
	*/
	STcpServer(uint8_t listenQueueSize = 30);

	//! Distructor
	virtual ~STcpServer();

	//! Abort the server, closing the socket
	void abort();

	//! Get the socket descriptor. It can be -1 if the socket in not valid.
	inline int getSocketDescriptor() const { return sock; }

	//! Listening check. \return true if the socket is lisntening, false otherwise
	inline bool isListening() const { return (sock != -1); }

	/**
		\brief Set up the server to listen for incoming connections on any address "0.0.0.0" and port \b port.
		\param port the server port (zero is not valid)
		\param error type if false is returned
		\return Retutrns true on success, otherwise returns false

		\note this function blocks the caller. This class should be used froma a dedicated Thread
	 */
	bool listen(uint16_t port, SAbstractSocket::SocketError *error = NULL);
};

#endif // STCPSERVER_H
