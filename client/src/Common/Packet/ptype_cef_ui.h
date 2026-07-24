#ifndef __PTYPE_CEF_UI_H__
#define __PTYPE_CEF_UI_H__

#include "ptype_base.h"

// Keep this value synchronized with the server. Appending it after
// MSG_EX_NOTICE preserves every legacy protocol identifier.
enum
{
	MSG_EX_CEF_UI = MSG_EX_NOTICE + 1,
};

enum MSG_EX_CEF_UI_ACTION
{
	MSG_EX_CEF_UI_OPEN = 1,
	MSG_EX_CEF_UI_CLOSE,
};

static const int CEF_UI_MAX_PARAMETER_LENGTH = 1024;
static const int CEF_UI_MAX_ROUTE_LENGTH = 31;
static const int CEF_UI_MAX_PLAYER_LENGTH = 50;

#pragma pack(push, 1)
struct pTypeCefUi : public pTypeThirdBase
{
	int width;
	int height;
	char route[CEF_UI_MAX_ROUTE_LENGTH + 1];
	char parameters[CEF_UI_MAX_PARAMETER_LENGTH + 1];
	char playerName[CEF_UI_MAX_PLAYER_LENGTH + 1];
};
#pragma pack(pop)

#endif
