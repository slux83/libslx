/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SABSTRACTSOCKET_H
#define SABSTRACTSOCKET_H

#include <memory>
#include <string>
#include <stdint.h>
#include "../global/sglobal.h"

/*!
	\brief Abstract socket class
*/
class SAbstractSocket
{
public:

	//! Socket error enumeration
	enum SocketError
	{
		SocketErrorUnknown,
		SocketErrorCannotResolve,
		SocketErrorCannotCreateSocket,
		SocketErrorConnectionRefused
	};

	//! Enumeratino of Protocol Type
	enum SocketType
	{
		SocketTypeUnk,
		SocketTypeTcp,
		SocketTypeUdp
	};

protected:
	SocketType socketType;

public:
	//! Constructor
	SAbstractSocket(SAbstractSocket::SocketType type);

	//! Distructor
	virtual ~SAbstractSocket() {}

	//! Abort and close the socket discarding any pending data in the write buffer.
	virtual void abort() {}

	/*! Send data using the socket.
		\return the number of wrote bytes, -1 means error
	*/
	virtual int64_t writeData(const char* data, size_t size) = 0;

	/*! Read data using the socket with a maximum of maxSize.
		\return the number of read bytes, -1 means error
	*/
	virtual int64_t readData(char* data, uint32_t maxSize) = 0;

};

#endif // SABSTRACTSOCKET_H
