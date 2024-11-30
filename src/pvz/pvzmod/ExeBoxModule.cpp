#include <Windows.h>

#include "framework/log/Log.h"
#include "framework/hook/ImportHook.h"
#include "framework/hook/ForwardHook.h"

#include "exebox/api/IExeBoxApi.h"
#include "framework/hook/ThisCall.h"


namespace
{

LPCSTR getCommandLineHook()
{
	return "";
}

void promptQuit(void*)
{
	framework::log::info() << "Bro tryna quit lil nigga";
}

}

std::optional<framework::hook::ForwardHook> quitHook = {};

extern "C"
__declspec(dllexport) void install(exebox::IExeBoxApi* api)
{
	framework::log::info() << "Installing PvZ module";

	const auto mainModule = api->moduleManager().mainModuleProvider().mainModule();

	// The executable requires that no command line argument are passed to it to start properly.
	// disable that by hooking that module.
	framework::hook::createImportHook(*mainModule, "KERNEL32.dll")
		.hook("GetCommandLineA", getCommandLineHook);

	quitHook = framework::hook::ForwardHook(0x004583F0, framework::hook::ThisCall<void()>(promptQuit));
}

extern "C"
__declspec(dllexport) void uninstall(exebox::IExeBoxApi* api)
{
	framework::log::info() << "Removing PVZ module";
}
