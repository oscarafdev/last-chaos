#ifndef __PACKET_SIZE_VALIDATION_H__
#define __PACKET_SIZE_VALIDATION_H__

#include <stddef.h>
#include <stdint.h>

#include "NetMsg.h"

namespace PacketSizeValidation
{
	inline bool IsValidCNetMsgPayload(
		uint32_t payloadSize,
		size_t trailerSize)
	{
		const size_t availableSize =
			MAX_MESSAGE_SIZE - sizeof(MsgHeader);
		if (trailerSize > availableSize)
			return false;

		const size_t maximumPayloadSize =
			availableSize - trailerSize;
		return payloadSize >= static_cast<uint32_t>(MAX_MESSAGE_TYPE)
			&& static_cast<size_t>(payloadSize) <= maximumPayloadSize;
	}
}

#endif
