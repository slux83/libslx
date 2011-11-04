/******************************************************************************
[FILE_HEADER_BEGIN]
[FILE_HEADER_END]
******************************************************************************/

#ifndef SNETWORKADDRESS_H
#define SNETWORKADDRESS_H

#include <string>

/*!
	\brief Network address that supports IPV4 and IPV6 procotols
	\todo
*/
class SNetworkAddress
{
public:
	//! Build an host address passing the host by string.
	SNetworkAddress(const std::string &host);
};

#endif // SNETWORKADDRESS_H
