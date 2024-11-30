#include "runner/runtime/module/manager/ModuleManager.h"
#include "runner/runtime/module/modules/MockModule.h"

#include "runner/steam/FakeSteamApi.h"

#include "runner/Log.h"

namespace runner::steam {

namespace {

bool __cdecl SteamAPI_Init()
{
	framework::log::info() << "Init";
	return true;
}

void __cdecl SteamAPI_RunCallbacks()
{
	framework::log::info() << "Run callbacks";
	return;
}

void *SteamClient()
{
	framework::log::info() << "Client";
	return nullptr;
}

void *SteamUser()
{
	framework::log::info() << "User";
	return nullptr;
}

void *SteamUserStats()
{
	framework::log::info() << "User stats";
	return nullptr;
}

void *SteamRemoteStorage()
{
	framework::log::info() << "User stats";
	return nullptr;
}

}

runtime::Module addFakeSteamApi()
{
	auto module = std::make_unique<runtime::MockModule>("steam_api.dll");
	module->addExport("SteamAPI_Init", SteamAPI_Init)
		.addExport("SteamAPI_RunCallbacks", SteamAPI_RunCallbacks)
		.addExport("SteamUser", SteamUser)
		.addExport("SteamUserStats", SteamUserStats)
		.addExport("SteamRemoteStorage", SteamRemoteStorage)
		.addExport("SteamClient", SteamClient);

	(void)runtime::moduleManager::registerModule(std::move(module));
	return runtime::moduleManager::getModule("steam_api.dll").value();
}


}
