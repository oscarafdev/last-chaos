#ifndef LASTCHAOS_CEF_APPLICATION_H
#define LASTCHAOS_CEF_APPLICATION_H

#include "include/cef_app.h"
#include "include/cef_command_line.h"
#include "include/cef_scheme.h"

class CefApplication final : public CefApp
{
public:
	CefApplication() = default;

	void OnRegisterCustomSchemes(
		CefRawPtr<CefSchemeRegistrar> registrar) override
	{
		registrar->AddCustomScheme(
			"lcui",
			CEF_SCHEME_OPTION_STANDARD |
				CEF_SCHEME_OPTION_SECURE |
				CEF_SCHEME_OPTION_CORS_ENABLED |
				CEF_SCHEME_OPTION_FETCH_ENABLED);
	}

	void OnBeforeCommandLineProcessing(
		const CefString& processType,
		CefRefPtr<CefCommandLine> commandLine) override
	{
		// The legacy DirectX 9 renderer and Chromium can compete for GPU state
		// on older drivers. Software compositing is the stable baseline for UI.
		commandLine->AppendSwitch("disable-gpu");
	}

private:
	IMPLEMENT_REFCOUNTING(CefApplication);
	DISALLOW_COPY_AND_ASSIGN(CefApplication);
};

#endif
