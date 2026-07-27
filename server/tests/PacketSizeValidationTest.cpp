#include <assert.h>
#include <stdint.h>

#include "PacketSizeValidation.h"

int main()
{
	const uint32_t minimumPayload = MAX_MESSAGE_TYPE;
	const uint32_t maximumPayload =
		MAX_MESSAGE_SIZE - sizeof(MsgHeader);
	const uint32_t maximumPayloadWithCrc =
		maximumPayload - sizeof(int);

	assert(!PacketSizeValidation::IsValidCNetMsgPayload(0, 0));
	assert(PacketSizeValidation::IsValidCNetMsgPayload(minimumPayload, 0));
	assert(PacketSizeValidation::IsValidCNetMsgPayload(maximumPayload, 0));
	assert(!PacketSizeValidation::IsValidCNetMsgPayload(
		maximumPayload + 1,
		0));

	assert(PacketSizeValidation::IsValidCNetMsgPayload(
		maximumPayloadWithCrc,
		sizeof(int)));
	assert(!PacketSizeValidation::IsValidCNetMsgPayload(
		maximumPayloadWithCrc + 1,
		sizeof(int)));

	// El valor -1 recibido por red se interpreta como 0xFFFFFFFF.
	assert(!PacketSizeValidation::IsValidCNetMsgPayload(
		UINT32_MAX,
		0));
	assert(!PacketSizeValidation::IsValidCNetMsgPayload(
		UINT32_MAX,
		sizeof(int)));
	return 0;
}
