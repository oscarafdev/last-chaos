#include "CefApplication.h"
#include "include/cef_app.h"

#include <windows.h>

int APIENTRY wWinMain(
	HINSTANCE instance,
	HINSTANCE previousInstance,
	wchar_t* commandLine,
	int commandShow)
{
	CefMainArgs mainArgs(instance);
	return CefExecuteProcess(mainArgs, new CefApplication(), nullptr);
}
