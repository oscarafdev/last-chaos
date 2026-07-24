#ifndef LASTCHAOS_CEF_UI_MESSAGES_H
#define LASTCHAOS_CEF_UI_MESSAGES_H

#include <windows.h>

// Private messages posted by CWebPage.dll to the game-owned panel window.
// Keep the transport small and explicit so future CEF views can reuse it.
constexpr UINT WM_LC_CEF_DRAG_BEGIN = WM_APP + 0x4C0;
constexpr UINT WM_LC_CEF_DRAG_MOVE = WM_APP + 0x4C1;
constexpr UINT WM_LC_CEF_DRAG_END = WM_APP + 0x4C2;
constexpr UINT WM_LC_CEF_CLOSE = WM_APP + 0x4C3;

#endif
