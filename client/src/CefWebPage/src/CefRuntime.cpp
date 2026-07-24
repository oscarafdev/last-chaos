#include "CefRuntime.h"
#include "CefApplication.h"

#include "include/base/cef_callback.h"
#include "include/base/cef_bind.h"
#include "include/cef_app.h"
#include "include/wrapper/cef_closure_task.h"

#include <algorithm>
#include <filesystem>

namespace
{
std::filesystem::path ModuleDirectory()
{
	HMODULE module = NULL;
	GetModuleHandleExW(
		GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
			GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
		reinterpret_cast<LPCWSTR>(&ModuleDirectory),
		&module);

	wchar_t path[MAX_PATH] = {};
	GetModuleFileNameW(module, path, MAX_PATH);
	return std::filesystem::path(path).parent_path();
}

}

CefRuntime& CefRuntime::Instance()
{
	static CefRuntime runtime;
	return runtime;
}

bool CefRuntime::Embed(HWND parent)
{
	if (parent == NULL || !Initialize())
		return false;

	RECT bounds = {};
	GetClientRect(parent, &bounds);

	{
		std::lock_guard<std::mutex> lock(stateMutex_);
		parent_ = parent;
	}

	CefPostTask(TID_UI, base::BindOnce(
		&CefRuntime::CreateBrowserOnUiThread,
		base::Unretained(this),
		parent,
		static_cast<int>(std::max(1L, bounds.right - bounds.left)),
		static_cast<int>(std::max(1L, bounds.bottom - bounds.top))));
	return true;
}

void CefRuntime::Unembed(HWND parent)
{
	CefRefPtr<CefBrowserClient> client;
	{
		std::lock_guard<std::mutex> lock(stateMutex_);
		if (parent != parent_)
			return;
		client = client_;
		client_ = nullptr;
		parent_ = NULL;
	}

	if (client)
		client->Close();
}

void CefRuntime::Navigate(HWND parent, const std::string& url)
{
	const std::string resolvedUrl = ResolveUrl(url);
	CefRefPtr<CefBrowserClient> client;
	{
		std::lock_guard<std::mutex> lock(stateMutex_);
		if (parent != parent_)
			return;
		pendingUrl_ = resolvedUrl;
		client = client_;
	}

	if (client)
		client->Navigate(resolvedUrl);
}

void CefRuntime::Resize(HWND parent, int width, int height)
{
	CefRefPtr<CefBrowserClient> client;
	{
		std::lock_guard<std::mutex> lock(stateMutex_);
		if (parent != parent_)
			return;
		client = client_;
	}

	if (client)
		client->Resize(std::max(1, width), std::max(1, height));
}

bool CefRuntime::Initialize()
{
	std::call_once(initializeFlag_, [this]()
	{
		const std::filesystem::path moduleDirectory = ModuleDirectory();
		const std::filesystem::path subprocess = moduleDirectory / "CefSubprocess.exe";
		const std::filesystem::path cache =
			std::filesystem::temp_directory_path() / "LastChaos" / "CefCache";

		CefMainArgs mainArgs(GetModuleHandleW(nullptr));
		CefSettings settings;
		settings.no_sandbox = true;
		settings.multi_threaded_message_loop = true;
		settings.persist_session_cookies = true;
		CefString(&settings.browser_subprocess_path) = subprocess.wstring();
		CefString(&settings.root_cache_path) = cache.wstring();
		CefString(&settings.cache_path) = cache.wstring();

		initialized_ = CefInitialize(
			mainArgs, settings, new CefApplication(), nullptr);
	});

	return initialized_;
}

void CefRuntime::CreateBrowserOnUiThread(
	HWND parent,
	int width,
	int height)
{
	CefWindowInfo windowInfo;
	windowInfo.SetAsChild(parent, CefRect(0, 0, width, height));
	windowInfo.runtime_style = CEF_RUNTIME_STYLE_ALLOY;

	CefBrowserSettings browserSettings;
	CefRefPtr<CefBrowserClient> client;
	{
		std::lock_guard<std::mutex> lock(stateMutex_);
		if (parent != parent_)
			return;
		client = new CefBrowserClient(
			pendingUrl_, (ModuleDirectory() / "cef-ui").wstring());
		client_ = client;
	}

	CefBrowserHost::CreateBrowser(
		windowInfo, client, "about:blank", browserSettings, nullptr, nullptr);
}

std::string CefRuntime::ResolveUrl(const std::string& url) const
{
	const std::string testRoute = "lcui://test";
	if (url.compare(0, testRoute.size(), testRoute) == 0)
		return url;

	if (url.compare(0, 8, "https://") == 0 ||
		url.compare(0, 7, "http://") == 0 ||
		url.compare(0, 8, "file:///") == 0)
	{
		return url;
	}

	return "about:blank";
}
