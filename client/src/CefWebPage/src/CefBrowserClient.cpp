#include "CefBrowserClient.h"

#include "include/base/cef_callback.h"
#include "include/base/cef_bind.h"
#include "include/cef_stream.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include <windows.h>

CefBrowserClient::CefBrowserClient(std::string initialUrl, std::wstring uiRoot)
	: pendingUrl_(std::move(initialUrl)),
	  uiRoot_(std::move(uiRoot))
{
}

CefRefPtr<CefLifeSpanHandler> CefBrowserClient::GetLifeSpanHandler()
{
	return this;
}

CefRefPtr<CefRequestHandler> CefBrowserClient::GetRequestHandler()
{
	return this;
}

void CefBrowserClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	browser_ = browser;
	if (!pendingUrl_.empty())
		NavigateOnUiThread(std::move(pendingUrl_));
}

void CefBrowserClient::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
	CEF_REQUIRE_UI_THREAD();
	browser_ = nullptr;
}

CefRefPtr<CefResourceRequestHandler>
CefBrowserClient::GetResourceRequestHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request,
	bool isNavigation,
	bool isDownload,
	const CefString& requestInitiator,
	bool& disableDefaultHandling)
{
	const std::string url = request->GetURL();
	const std::string testRoute = "lcui://test";
	if (url.compare(0, testRoute.size(), testRoute) != 0)
	{
		return nullptr;
	}

	disableDefaultHandling = true;
	return this;
}

CefRefPtr<CefResourceHandler> CefBrowserClient::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	const std::string url = request->GetURL();
	const std::string testRoute = "lcui://test";
	if (url.compare(0, testRoute.size(), testRoute) != 0)
		return nullptr;

	const std::wstring path = uiRoot_ + L"\\test\\index.html";
	CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForFile(path);
	if (!stream)
		return nullptr;

	return new CefStreamResourceHandler("text/html", stream);
}

void CefBrowserClient::Navigate(std::string url)
{
	CefPostTask(TID_UI, base::BindOnce(
		&CefBrowserClient::NavigateOnUiThread, CefRefPtr<CefBrowserClient>(this), std::move(url)));
}

void CefBrowserClient::Resize(int width, int height)
{
	CefPostTask(TID_UI, base::BindOnce(
		&CefBrowserClient::ResizeOnUiThread, CefRefPtr<CefBrowserClient>(this), width, height));
}

void CefBrowserClient::Close()
{
	CefPostTask(TID_UI, base::BindOnce(
		&CefBrowserClient::CloseOnUiThread, CefRefPtr<CefBrowserClient>(this)));
}

void CefBrowserClient::NavigateOnUiThread(std::string url)
{
	CEF_REQUIRE_UI_THREAD();
	if (!browser_)
	{
		pendingUrl_ = std::move(url);
		return;
	}

	browser_->GetMainFrame()->LoadURL(url);
}

void CefBrowserClient::ResizeOnUiThread(int width, int height)
{
	CEF_REQUIRE_UI_THREAD();
	if (!browser_)
		return;

	HWND browserWindow = browser_->GetHost()->GetWindowHandle();
	if (browserWindow != NULL)
		MoveWindow(browserWindow, 0, 0, width, height, TRUE);
}

void CefBrowserClient::CloseOnUiThread()
{
	CEF_REQUIRE_UI_THREAD();
	if (browser_)
		browser_->GetHost()->CloseBrowser(true);
}
