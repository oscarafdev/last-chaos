#ifndef LASTCHAOS_CEF_RUNTIME_H
#define LASTCHAOS_CEF_RUNTIME_H

#include "CefBrowserClient.h"

#include <windows.h>

#include <mutex>
#include <string>

class CefRuntime final
{
public:
	static CefRuntime& Instance();

	bool Embed(HWND parent);
	void Unembed(HWND parent);
	void Navigate(HWND parent, const std::string& url);
	void Resize(HWND parent, int width, int height);

private:
	CefRuntime() = default;

	bool Initialize();
	void CreateBrowserOnUiThread(HWND parent, int width, int height);
	std::string ResolveUrl(const std::string& url) const;

	std::once_flag initializeFlag_;
	bool initialized_ = false;
	std::mutex stateMutex_;
	HWND parent_ = NULL;
	CefRefPtr<CefBrowserClient> client_;
	std::string pendingUrl_ = "about:blank";
};

#endif
