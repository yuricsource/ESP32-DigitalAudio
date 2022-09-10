#pragma once

#include <cstring>
#include <cstdint>
#include "FastDelegate.h"
#include "lwip/ip_addr.h"
#include "CommonTypes.h"

namespace ConnectionTypes
{

using namespace fastdelegate;
using Common::IpAddress;

enum class ConnectionState
{
	ConnectionNotConfigured = 0,
	Connecting = 1,
	Connected = 2,
	Disconnected = 3
};

enum class ConnectionChangeReason
{
	None,
	Error,
	Timeout,
	Aborted,
	Reset,
	Closed
};

struct RemoteConnection
{
	uint16_t Port;
	IpAddress Address;
};

} // namespace Common