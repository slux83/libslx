/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SABSTRACTSOCKET_H
#define SABSTRACTSOCKET_H

/*!
	\brief Abstract socket class
	\todo
*/
class SAbstractSocket
{
public:

	//! Enumeratino of Protocol Type
	enum ProtocolType
	{
		ProtocolTypeUnk,
		ProtocolTypeIpv4,
		ProtocolTypeIpv6
	};

protected:
	ProtocolType protocolType;

public:
	//! Constructor
	SAbstractSocket(SAbstractSocket::ProtocolType type);

	//! Distructor
	virtual ~SAbstractSocket() {}
};

#endif // SABSTRACTSOCKET_H
