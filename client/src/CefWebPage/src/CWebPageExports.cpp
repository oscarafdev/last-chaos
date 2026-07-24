#include "CefRuntime.h"

#include <windows.h>

extern "C" HRESULT WINAPI EmbedBrowserObject(HWND parent)
{
	return CefRuntime::Instance().Embed(parent) ? S_OK : E_FAIL;
}

extern "C" HRESULT WINAPI UnEmbedBrowserObject(HWND parent)
{
	CefRuntime::Instance().Unembed(parent);
	return S_OK;
}

extern "C" HRESULT WINAPI DisplayHTMLPage(HWND parent, const char* url)
{
	if (url == nullptr)
		return E_INVALIDARG;

	CefRuntime::Instance().Navigate(parent, url);
	return S_OK;
}

extern "C" void WINAPI ResizeBrowser(HWND parent, DWORD width, DWORD height)
{
	CefRuntime::Instance().Resize(
		parent, static_cast<int>(width), static_cast<int>(height));
}
