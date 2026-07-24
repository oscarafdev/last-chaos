#ifndef LASTCHAOS_CEF_BROWSER_CLIENT_H
#define LASTCHAOS_CEF_BROWSER_CLIENT_H

#include "include/cef_client.h"
#include "include/cef_load_handler.h"
#include "include/cef_request_handler.h"
#include "include/cef_resource_request_handler.h"

#include <windows.h>

#include <string>

class CefBrowserClient final :
	public CefClient,
	public CefLifeSpanHandler,
	public CefLoadHandler,
	public CefRequestHandler,
	public CefResourceRequestHandler
{
public:
	CefBrowserClient(HWND parent, std::string initialUrl, std::wstring uiRoot);

	CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() override;
	CefRefPtr<CefLoadHandler> GetLoadHandler() override;
	CefRefPtr<CefRequestHandler> GetRequestHandler() override;
	void OnAfterCreated(CefRefPtr<CefBrowser> browser) override;
	void OnBeforeClose(CefRefPtr<CefBrowser> browser) override;
	void OnLoadEnd(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		int httpStatusCode) override;
	CefRefPtr<CefResourceRequestHandler> GetResourceRequestHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request,
		bool isNavigation,
		bool isDownload,
		const CefString& requestInitiator,
		bool& disableDefaultHandling) override;
	CefRefPtr<CefResourceHandler> GetResourceHandler(
		CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		CefRefPtr<CefRequest> request) override;

	void Navigate(std::string url);
	void Resize(int width, int height);
	void Close();

private:
	void NavigateOnUiThread(std::string url);
	void ResizeOnUiThread(int width, int height);
	void CloseOnUiThread();
	bool HandleUiAction(const std::string& url);

	HWND parent_;
	CefRefPtr<CefBrowser> browser_;
	std::string pendingUrl_;
	std::wstring uiRoot_;

	IMPLEMENT_REFCOUNTING(CefBrowserClient);
	DISALLOW_COPY_AND_ASSIGN(CefBrowserClient);
};

#endif
