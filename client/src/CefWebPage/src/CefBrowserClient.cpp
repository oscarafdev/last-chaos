#include "CefBrowserClient.h"
#include "CefUiMessages.h"

#include "include/base/cef_callback.h"
#include "include/base/cef_bind.h"
#include "include/cef_stream.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "include/wrapper/cef_stream_resource_handler.h"

#include <windows.h>

CefBrowserClient::CefBrowserClient(
	HWND parent,
	std::string initialUrl,
	std::wstring uiRoot)
	: parent_(parent),
	  pendingUrl_(std::move(initialUrl)),
	  uiRoot_(std::move(uiRoot))
{
}

CefRefPtr<CefLifeSpanHandler> CefBrowserClient::GetLifeSpanHandler()
{
	return this;
}

CefRefPtr<CefLoadHandler> CefBrowserClient::GetLoadHandler()
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

void CefBrowserClient::OnLoadEnd(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	int httpStatusCode)
{
	CEF_REQUIRE_UI_THREAD();
	if (!frame->IsMain())
		return;

	HWND browserWindow = browser->GetHost()->GetWindowHandle();
	if (browserWindow != NULL)
	{
		InvalidateRect(browserWindow, nullptr, FALSE);
		UpdateWindow(browserWindow);
	}
	if (IsWindow(parent_))
	{
		// The legacy DirectX 9 host may not repaint a newly loaded modern CEF
		// child until its frame changes. Refresh without moving or activating it.
		SetWindowPos(
			parent_,
			nullptr,
			0,
			0,
			0,
			0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
				SWP_NOACTIVATE | SWP_FRAMECHANGED);
		RedrawWindow(
			parent_,
			nullptr,
			nullptr,
			RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	}
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
	const std::string actionRoute = "lcui://action/";
	if (url.compare(0, testRoute.size(), testRoute) != 0 &&
		url.compare(0, actionRoute.size(), actionRoute) != 0)
	{
		return nullptr;
	}

	if (url.compare(0, actionRoute.size(), actionRoute) == 0)
		HandleUiAction(url);

	disableDefaultHandling = false;
	return this;
}

CefRefPtr<CefResourceHandler> CefBrowserClient::GetResourceHandler(
	CefRefPtr<CefBrowser> browser,
	CefRefPtr<CefFrame> frame,
	CefRefPtr<CefRequest> request)
{
	const std::string url = request->GetURL();
	const std::string testRoute = "lcui://test";
	const std::string actionRoute = "lcui://action/";
	if (url.compare(0, actionRoute.size(), actionRoute) == 0)
	{
		static const char response[] = "ok";
		CefRefPtr<CefStreamReader> stream =
			CefStreamReader::CreateForData(
				const_cast<char*>(response), sizeof(response) - 1);
		return new CefStreamResourceHandler("text/plain", stream);
	}

	if (url.compare(0, testRoute.size(), testRoute) != 0)
		return nullptr;

	const std::wstring path = uiRoot_ + L"\\test\\index.html";
	CefRefPtr<CefStreamReader> stream = CefStreamReader::CreateForFile(path);
	if (!stream)
		return nullptr;

	return new CefStreamResourceHandler("text/html", stream);
}

bool CefBrowserClient::HandleUiAction(const std::string& url)
{
	const std::string actionRoute = "lcui://action/";
	const size_t actionEnd = url.find('?', actionRoute.size());
	const std::string action = url.substr(
		actionRoute.size(), actionEnd - actionRoute.size());
	UINT message = 0;

	if (action == "drag-start")
		message = WM_LC_CEF_DRAG_BEGIN;
	else if (action == "drag-move")
		message = WM_LC_CEF_DRAG_MOVE;
	else if (action == "drag-end")
		message = WM_LC_CEF_DRAG_END;
	else if (action == "close")
		message = WM_LC_CEF_CLOSE;
	else
		return false;

	if (IsWindow(parent_))
		PostMessage(parent_, message, 0, 0);
	return true;
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
